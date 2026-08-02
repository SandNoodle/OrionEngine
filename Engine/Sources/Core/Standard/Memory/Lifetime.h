#pragma once

#include "OrionEngine.h"

#include "Core/Standard/TypeTraits.h"

#include <new>

namespace Orion::Engine::Memory
{
	/**
	 * @brief Default constructs \p count into a memory, effectively begins C++' lifetime for these elements.
	 * @param dst Destination address to where copy the elements.
	 * @param count Number of elements to construct.
	 */
	template <typename ItemType, typename SizeType = USize>
		requires(sizeof(ItemType) > 0 && !!IsZeroConstructible<ItemType>)
	ORION_FORCE_INLINE void DefaultConstructItems(ItemType* dst, SizeType count) noexcept
	{
		if (count) {
			Platform::MemorySet(dst, 0, sizeof(ItemType) * count);
		}
	}
	template <typename ItemType, typename SizeType = USize>
		requires(sizeof(ItemType) > 0 && !IsZeroConstructible<ItemType>)
	ORION_FORCE_INLINE void DefaultConstructItems(ItemType* dst, SizeType count) noexcept
	{
		while (count--) {
			::new (static_cast<void*>(dst)) ItemType;
			++dst;
		}
	}

	/**
	 * @brief Constructs \p count elements into a memory, effectively begins C++'s lifetime for these elements.
	 *
	 * @param dst Destination address to where copy the elements.
	 * @param src Source address of the first element to pass.
	 * @param count Number of elements to copy from \p src.
	 */
	template <typename SourceItemType, typename DestinationItemType = SourceItemType, typename SizeType = USize>
		requires(sizeof(SourceItemType) > 0 && sizeof(DestinationItemType) > 0
	             && !IsTriviallyConstructible<DestinationItemType>)
	ORION_FORCE_INLINE void ConstructItems(DestinationItemType* dst, const SourceItemType* src, SizeType count) noexcept
	{
		while (count--) {
			::new (static_cast<void*>(dst)) DestinationItemType(*src);
			++src;
			++dst;
		}
	}
	template <typename SourceItemType, typename DestinationItemType = SourceItemType, typename SizeType = USize>
		requires(sizeof(SourceItemType) > 0 && sizeof(DestinationItemType) > 0
	             && !!IsTriviallyConstructible<DestinationItemType>)
	ORION_FORCE_INLINE void ConstructItems(DestinationItemType* dst, const SourceItemType* src, SizeType count) noexcept
	{
		if (count) {
			Platform::MemoryCopy(dst, src, sizeof(SourceItemType) * count);
		}
	}

	/**
	 * @brief Constructs a given element into a memory, effectively begins C++'s lifetime for it.
	 * @param dst Destination address where the element will be constructed.
	 * @param item Element to construct.
	 */
	template <typename SourceItemType, typename DestinationItemType = SourceItemType, typename SizeType = USize>
	ORION_FORCE_INLINE void ConstructItem(DestinationItemType* dst, const SourceItemType& item) noexcept
	{
		ConstructItems(dst, &item, 1);
	}

	/**
	 * @brief Constructs a given element into a memory, effectively begins C++'s lifetime for it.
	 * @param dst Destination address where the element will be constructed.
	 * @param item Element to construct.
	 */
	template <typename SourceItemType, typename DestinationItemType = SourceItemType, typename SizeType = USize>
	ORION_FORCE_INLINE void ConstructItem(DestinationItemType* dst, SourceItemType&& item) noexcept
	{
		ConstructItems(dst, &item, 1);
	}

	/**
	 * @brief Destructs \p count of elements from a given memory region, effectively ends C++'s lifetime for them.
	 * @param src Address where to the elements will be destroyed.
	 * @param count Number of elements to destroy.
	 */
	template <typename ItemType, typename SizeType>
		requires(sizeof(ItemType) > 0)
	ORION_FORCE_INLINE void DestructItems(ItemType* src, SizeType count) noexcept
	{
		if (!IsTriviallyDestructible<ItemType>) {
			while (count--) {
				src->ItemType::~ItemType();
			}
		}
	}
}  // namespace Orion::Engine::Memory
