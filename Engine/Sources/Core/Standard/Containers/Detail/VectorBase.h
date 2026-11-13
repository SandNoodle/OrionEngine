#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Math/Math.h"
#include "Core/Standard/Memory/Allocators/Allocator.h"
#include "Core/Standard/Utility/MathUtils.h"

namespace Orion::Engine::Detail
{
	/// @brief Base class for all vector types - it's only concern is to perform memory operations (allocations;
	/// deallocations).
	/// @tparam T Type to be stored.
	/// @tparam Allocator Type of the allocator to perform the memory operations.
	template <typename T, Memory::AllocatorKind Allocator>
	class VectorBase
	{
		public:
		using ValueType          = T;
		using SizeType           = USize;
		using AllocatorType      = Allocator;
		using PointerType        = ValueType*;
		using ConstPointerType   = const ValueType*;
		using ReferenceType      = ValueType&;
		using ConstReferenceType = const ValueType&;

		public:
		static constexpr SizeType k_initial_capacity = ORION_CONTAINERS_VECTOR_INITIAL_CAPACITY;

		protected:
		AllocatorType _allocator;
		PointerType _data{ nullptr };
		SizeType _capacity{ 0UL };
		SizeType _size{ 0UL };

		public:
		constexpr explicit VectorBase() noexcept;
		constexpr explicit VectorBase(const AllocatorType& allocator) noexcept;
		constexpr explicit VectorBase(SizeType initial_capacity, const AllocatorType& allocator) noexcept;
		constexpr ~VectorBase() noexcept;

		protected:
		constexpr AllocatorType& GetAllocator() noexcept;
		constexpr const AllocatorType& GetAllocator() const noexcept;

		protected:
		constexpr void EnsureCapacity(SizeType requested_capacity) noexcept;
	};

	// -- Implementation.
	template <typename T, Memory::AllocatorKind Allocator>
	constexpr VectorBase<T, Allocator>::VectorBase() noexcept
		: _allocator(AllocatorType{}),
		  _data(Memory::AllocateCount<ValueType>(_allocator, k_initial_capacity)),
		  _capacity(k_initial_capacity),
		  _size(0UL)
	{
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr VectorBase<T, Allocator>::VectorBase(const AllocatorType& allocator) noexcept
		: _allocator(allocator),
		  _data(Memory::AllocateCount<ValueType>(_allocator, k_initial_capacity)),
		  _capacity(k_initial_capacity),
		  _size(0UL)
	{
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr VectorBase<T, Allocator>::VectorBase(SizeType initial_capacity, const AllocatorType& allocator) noexcept
		: _allocator(allocator), _data(nullptr), _capacity(0UL), _size(0UL)
	{
		_capacity = Math::Max(initial_capacity, k_initial_capacity);
		_data     = Memory::AllocateCount<ValueType>(_allocator, _capacity);
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr VectorBase<T, Allocator>::~VectorBase() noexcept
	{
		if (_data) {
			_allocator.Free(_data);
			_data = nullptr;
		}
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto VectorBase<T, Allocator>::GetAllocator() noexcept -> AllocatorType&
	{
		return _allocator;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto VectorBase<T, Allocator>::GetAllocator() const noexcept -> const AllocatorType&
	{
		return _allocator;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto VectorBase<T, Allocator>::EnsureCapacity(SizeType requested_capacity) noexcept -> void
	{
		ORION_ASSERT_DEBUG_SLOW(_data);

		if (_capacity > requested_capacity) {
			return;
		}

		SizeType new_capacity = ToNextPowerOfTwo(requested_capacity);
		PointerType new_data  = Memory::AllocateCount<ValueType>(_allocator, new_capacity);

		Memory::ConstructItems(new_data, _data, _size);
		Memory::DestructItems(_data, _size);

		_allocator.Free(_data);
		_data     = new_data;
		_capacity = new_capacity;
	}
}  // namespace Orion::Engine::Detail
