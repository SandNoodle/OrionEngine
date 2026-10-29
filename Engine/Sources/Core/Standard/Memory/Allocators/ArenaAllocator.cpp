#include "Core/Standard/Memory/Allocators/ArenaAllocator.h"

#include "Core/Log/Logger.h"
#include "Platform/Memory.h"

#include "Core/Assert.h"
#include "Core/Standard/Memory/Lifetime.h"
#include "Core/Standard/Utility/MathUtils.h"

namespace Orion::Engine::Memory
{
	struct ArenaRegion
	{
		ArenaRegionHandle previous;
		USize capacity;
		USize size;
		Byte* data;
	};

	static ArenaRegionHandle ArenaRegionCreate(USize block_size, ArenaRegionHandle previous_region)
	{
		USize arena_region_header_size = sizeof(ArenaRegion);
		USize arena_region_data_size   = block_size;
		USize arena_region_total_size  = arena_region_header_size + arena_region_data_size;

		ArenaRegion* region = static_cast<ArenaRegion*>(Platform::MemoryAllocate(arena_region_total_size));
		if (!region) {
			ORION_LOG_ERROR("ArenaAllocator failed to allocate '{}' bytes for the next memory block.", block_size);
			return nullptr;
		}

		ConstructItem(region);
		region->previous = previous_region;
		region->data     = reinterpret_cast<Byte*>(region) + arena_region_header_size;
		region->capacity = arena_region_data_size;
		region->size     = 0UL;
		return region;
	}

	// TODO(SandNoodle): Move this out.
	static USize AlignValueForward(USize value, USize alignment)
	{
		ORION_ASSERT_DEBUG(IsPowerOfTwo(alignment), "Alignment must be a power of two.");
		UIntPtr aligned_value = (value + alignment - 1) & ~(alignment - 1);
		ORION_ASSERT_DEBUG(aligned_value >= value);
		return aligned_value;
	}

	static void* ArenaRegionAllocateUnsafe(ArenaRegionHandle region, USize size_in_bytes)
	{
		ORION_ASSERT_DEBUG(region);
		Byte* ptr = region->data + region->size;
		region->size += size_in_bytes;
		return ptr;
	}

	ArenaAllocator::ArenaAllocator() : _regions_begin(nullptr), _regions_current(nullptr) {}

	ArenaAllocator::~ArenaAllocator()
	{
		ArenaRegionHandle region = _regions_current;
		while (region) {
			ArenaRegionHandle previous_region = region->previous;
			DestructItems(region, 1);
			Platform::MemoryFree(region);
			region = previous_region;
		}
	}

	void* ArenaAllocator::Allocate(SizeType size_in_bytes, SizeType alignment) noexcept
	{
		USize aligned_size_in_bytes   = AlignValueForward(size_in_bytes, alignment);
		SizeType requested_block_size = aligned_size_in_bytes <= k_default_block_size
		                                  ? k_default_block_size
		                                  : ToNextPowerOfTwo(aligned_size_in_bytes);

		if (!_regions_begin) [[unlikely]] {
			ArenaRegionHandle region = ArenaRegionCreate(requested_block_size, nullptr);
			if (!region) {
				ORION_LOG_ERROR("ArenaAllocator failed to allocate the first memory block ('{}' bytes)",
				                requested_block_size);
				return nullptr;
			}
			_regions_begin   = region;
			_regions_current = region;
		}

		if (_regions_current->size + aligned_size_in_bytes > _regions_current->capacity) {
			// If possible, prefer already allocated regions to fit the requested allocation...
			ArenaRegionHandle potential_region = _regions_current;
			while (potential_region) {
				if (potential_region->size + aligned_size_in_bytes <= potential_region->capacity) {
					return ArenaRegionAllocateUnsafe(potential_region, aligned_size_in_bytes);
				}
				potential_region = potential_region->previous;
			}

			// ...if not, just allocate a new one for it.
			_regions_current = ArenaRegionCreate(requested_block_size, _regions_current);
			if (!_regions_current) {
				ORION_LOG_ERROR("ArenaAllocator failed to allocate the next memory block ('{}' bytes)",
				                requested_block_size);
				return nullptr;
			}
		}

		return ArenaRegionAllocateUnsafe(_regions_current, aligned_size_in_bytes);
	}

	void ArenaAllocator::Free(void* ptr) noexcept
	{
		ORION_ASSERT_DEBUG(ptr, "Cannot perform ArenaAllactor::Free, because ptr is null.");
		// ArenaAllocator cannot free individual allocations.
	}

	void ArenaAllocator::FreeAll() noexcept
	{
		ArenaRegionHandle region = _regions_current;
		while (region) {
			region->size = 0UL;
			region       = region->previous;
		}
	}

	ArenaAllocator::SizeType ArenaAllocator::Size() const noexcept
	{
		SizeType size            = 0UL;
		ArenaRegionHandle region = _regions_current;
		while (region) {
			size += region->size;
			region = region->previous;
		}
		return size;
	}

	ArenaAllocator::SizeType ArenaAllocator::Capacity() const noexcept
	{
		SizeType capacity        = 0UL;
		ArenaRegionHandle region = _regions_current;
		while (region) {
			capacity += region->capacity;
			region = region->previous;
		}
		return capacity;
	}
}  // namespace Orion::Engine::Memory
