#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Memory/Allocators/Allocator.h"
#include "Core/Memory/Allocators/PlatformAllocator.h"
#include "Core/Standard/Algorithms/Sort.h"
#include "Core/Standard/Memory/Lifetime.h"
#include "Core/Standard/Util.h"
#include "Core/Standard/Utility/MoveAndForward.h"
#include "Platform/Memory.h"

#include <initializer_list>
#include <new>

namespace Orion::Engine
{
	/**
	 * @brief Represents a type-safe container capable of holding variable amount of elements.
	 * @tparam T Type to be stored.
	 * @tparam Allocator Allocator to be used with the Vector that will perform all the allocations.
	 */
	template <typename T, AllocatorKind Allocator = PlatformAllocator>
	class Vector
	{
		public:
		using ValueType          = T;
		using SizeType           = USize;
		using AllocatorType      = Allocator;
		using PointerType        = ValueType*;
		using ConstPointerType   = const ValueType*;
		using ReferenceType      = ValueType&;
		using ConstReferenceType = const ValueType&;

		static constexpr SizeType k_initial_capacity = 8UL;

		private:
		AllocatorType _allocator;
		PointerType _data;
		SizeType _capacity;
		SizeType _size;

		public:
		constexpr explicit Vector(SizeType initial_capacity      = k_initial_capacity,
		                          const AllocatorType& allocator = AllocatorType());
		constexpr Vector(std::initializer_list<ValueType>) noexcept;
		constexpr Vector(const Vector&) noexcept;
		constexpr Vector(Vector&&) noexcept;
		constexpr ~Vector();

		constexpr Vector& operator=(const Vector&) noexcept;
		constexpr Vector& operator=(Vector&&) noexcept;
		constexpr Vector& operator=(std::initializer_list<ValueType>) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType operator[](SizeType index) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType operator[](SizeType index) const noexcept;

		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Front() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType Front() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Back() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType Back() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr PointerType Data() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstPointerType Data() const noexcept;

		/// @brief Constructs new element at the back of the vector.
		template <typename... Args>
		constexpr void AddConstruct(Args&&... args) noexcept;

		/// @brief Constructs \p count default initialized elements at the back of the vector.
		constexpr void AddZeroed(SizeType count) noexcept;

		/// @brief Appends new element to the back of the vector (by Const-Reference).
		constexpr void Add(const ValueType& value) noexcept;

		/// @brief Appends new element to the back of the vector (by Move).
		constexpr void Add(ValueType&& value) noexcept;

		/// @brief Appends \p size new elements to the back of the vector.
		constexpr void AddRange(ConstPointerType begin, ConstPointerType end);

		/// @brief Removes the element from the back of the vector. Calls destructor.
		constexpr void RemoveBack() noexcept;

		/// @brief Removes the element at \p index from the vector. Calls destructor.
		constexpr void Remove(SizeType index) noexcept;

		/// @brief Reserves memory for holding at least \p size elements (might be more). Has no effect if capacity is
		/// already greater than \p size.
		constexpr void Reserve(SizeType size) noexcept;

		/// @brief Clears the vector from all the elements. Calls destructor on each of them.
		constexpr void Clear() noexcept;

		/// @brief Sorts the container using a given \p compare predicate function.
		template <typename CompareFn>
		constexpr void Sort(CompareFn&& compare) noexcept;

		/// @brief Verifies if the container is empty, i.e. can store only 0 elements.
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 IsEmpty() const noexcept;

		/// @brief Returns the amount of elements that can be stored in this container.
		[[nodiscard]] ORION_FORCE_INLINE constexpr SizeType Size() const noexcept;

		/// @brief Returns the amount of bytes that this container stores.
		[[nodiscard]] ORION_FORCE_INLINE constexpr SizeType ByteSize() const noexcept;

		/// @brief Returns the maximum amount of elements that can be currently stored in this container.
		[[nodiscard]] ORION_FORCE_INLINE constexpr SizeType Capacity() const noexcept;

		public:
		// NOLINTBEGIN(readability-identifier-naming)
		/** Required overload for the C++'s for range loops. */
		///@{
		[[nodiscard]] ORION_FORCE_INLINE constexpr PointerType begin() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstPointerType begin() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr PointerType end() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstPointerType end() const noexcept;
		///@}
		// NOLINTEND(readability-identifier-naming)

		private:
		constexpr void DoInitialize(SizeType initial_capacity) noexcept;
		constexpr void DoSwap(Vector& other) noexcept;
		constexpr void DoEnsureCapacity(SizeType new_capacity) noexcept;
	};

	// -- Implementation.
	template <typename T, AllocatorKind Allocator>
	constexpr Vector<T, Allocator>::Vector(SizeType initial_capacity, const AllocatorType& allocator)
		: _allocator(allocator)
	{
		DoInitialize(initial_capacity);
	}

	template <typename T, AllocatorKind Allocator>
	constexpr Vector<T, Allocator>::Vector(std::initializer_list<ValueType> list) noexcept : _allocator(Allocator{})
	{
		DoInitialize(list.size());
		ConstructItems<ValueType>(Data(), list.begin(), list.size());
		_size = list.size();
	}

	template <typename T, AllocatorKind Allocator>
	constexpr Vector<T, Allocator>::Vector(const Vector& other) noexcept
		: _allocator(other._allocator), _capacity(other._capacity), _size(other._size)

	{
		_data = static_cast<PointerType>(_allocator.Allocate(sizeof(ValueType) * _capacity, alignof(ValueType)));
		if constexpr (IsTriviallyCopyable<ValueType>) {
			Platform::MemoryCopy(_data, other._data, ByteSize());
		} else {
			ConstructItems(_data, other._data, Size());
		}
	}

	template <typename T, AllocatorKind Allocator>
	constexpr Vector<T, Allocator>::Vector(Vector&& other) noexcept : _data(nullptr), _capacity(0), _size(0)
	{
		DoSwap(other);
	}

	template <typename T, AllocatorKind Allocator>
	constexpr Vector<T, Allocator>::~Vector()
	{
		if (_data) {
			DestructItems(_data, _size);
			_allocator.Free(_data);
		}
	}
	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::operator=(const Vector& other) noexcept -> Vector&
	{
		if (this == &other) {
			return *this;
		}

		if (_data) {
			DestructItems(_data, _size);
			_allocator.Free(_data);
		}

		_allocator = other._allocator;
		_capacity  = other._capacity;
		_size      = other._size;
		_data      = static_cast<PointerType>(_allocator.Allocate(sizeof(ValueType) * _capacity, alignof(ValueType)));

		if constexpr (IsTriviallyCopyable<ValueType>) {
			Platform::MemoryCopy(_data, other._data, ByteSize());
		} else {
			ConstructItems(_data, other._data, Size());
		}

		return *this;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::operator=(Vector&& other) noexcept -> Vector&
	{
		if (this == &other) {
			return *this;
		}

		DoSwap(other);

		return *this;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::operator=(std::initializer_list<ValueType> list) noexcept -> Vector&
	{
		DestructItems(Data(), Size());
		ConstructItems<ValueType>(Data(), list.begin(), list.size());
		_size = list.size();
		return *this;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::operator[](SizeType index) noexcept -> ReferenceType
	{
		ORION_ASSERT_DEBUG(index < _size);
		return _data[index];
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::operator[](SizeType index) const noexcept -> ConstReferenceType
	{
		ORION_ASSERT_DEBUG(index < _size);
		return _data[index];
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Front() noexcept -> ReferenceType
	{
		return _data[0];
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Front() const noexcept -> ConstReferenceType
	{
		return _data[0];
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Back() noexcept -> ReferenceType
	{
		return _data[_size - 1];
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Back() const noexcept -> ConstReferenceType
	{
		return _data[_size - 1];
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Data() noexcept -> PointerType
	{
		return _data;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Data() const noexcept -> ConstPointerType
	{
		return _data;
	}

	template <typename T, AllocatorKind Allocator>
	template <typename... Args>
	constexpr auto Vector<T, Allocator>::AddConstruct(Args&&... args) noexcept -> void
	{
		DoEnsureCapacity(_size + 1);
		new (&_data[_size]) ValueType(Forward<Args>(args)...);
		_size += 1;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::AddZeroed(SizeType count) noexcept -> void
	{
		DoEnsureCapacity(_size + count);
		for (SizeType index = 0; index < count; ++index) {
			new (&_data[_size + index]) ValueType();
		}
		_size += count;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Add(const ValueType& value) noexcept -> void
	{
		DoEnsureCapacity(_size + 1);
		new (&_data[_size]) ValueType(value);
		_size += 1;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Add(ValueType&& value) noexcept -> void
	{
		DoEnsureCapacity(_size + 1);
		new (&_data[_size]) ValueType(Move(value));
		_size += 1;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::AddRange(ConstPointerType begin, ConstPointerType end) -> void
	{
		ORION_ASSERT_DEBUG(begin);
		ORION_ASSERT_DEBUG(end);
		ORION_ASSERT_DEBUG(begin <= end);
		SizeType size = static_cast<SizeType>(end - begin);
		DoEnsureCapacity(_size + size);
		for (ConstPointerType it = begin; it < end; ++it) {
			new (&_data[_size]) ValueType(*it);
			_size += 1;
		}
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::RemoveBack() noexcept -> void
	{
		ORION_ASSERT_DEBUG(_data);
		ORION_ASSERT_DEBUG(_size > 0);
		_data[--_size].~ValueType();
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Remove(SizeType index) noexcept -> void
	{
		ORION_ASSERT_DEBUG(_data);
		ORION_ASSERT_DEBUG(_size > 0);
		Swap(_data[index], _data[_size - 1]);
		_data[--_size].~ValueType();
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Reserve(SizeType size) noexcept -> void
	{
		DoEnsureCapacity(size);
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Clear() noexcept -> void
	{
		if constexpr (!IsTriviallyDestructible<ValueType>) {
			if (_data) {
				for (SizeType index = 0; index < _size; ++index) {
					_data[index].~ValueType();
				}
			}
		}
		_size = 0;
	}

	template <typename T, AllocatorKind Allocator>
	template <typename CompareFn>
	constexpr auto Vector<T, Allocator>::Sort(CompareFn&& compare) noexcept -> void
	{
		ORION_ASSERT_DEBUG(_data != nullptr);
		Algorithm::Sort(_data, _size, Forward<CompareFn>(compare));
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::IsEmpty() const noexcept -> Bool8
	{
		return _size == 0;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Size() const noexcept -> SizeType
	{
		return _size;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::ByteSize() const noexcept -> SizeType
	{
		return sizeof(ValueType) * Size();
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Capacity() const noexcept -> SizeType
	{
		return _capacity;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::begin() noexcept -> PointerType
	{
		return _data;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::begin() const noexcept -> ConstPointerType
	{
		return _data;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::end() noexcept -> PointerType
	{
		return _data + _size;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::end() const noexcept -> ConstPointerType
	{
		return _data + _size;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::DoInitialize(SizeType initial_capacity) noexcept -> void
	{
		initial_capacity       = ORION_MAX(initial_capacity, k_initial_capacity);
		SizeType size_in_bytes = sizeof(ValueType) * initial_capacity;
		SizeType alignment     = alignof(ValueType);
		_data                  = static_cast<PointerType>(_allocator.Allocate(size_in_bytes, alignment));
		_capacity              = initial_capacity;
		_size                  = 0;
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::DoSwap(Vector& other) noexcept -> void
	{
		Swap(_allocator, other._allocator);
		Swap(_data, other._data);
		Swap(_capacity, other._capacity);
		Swap(_size, other._size);
	}

	template <typename T, AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::DoEnsureCapacity(SizeType new_capacity) noexcept -> void
	{
		ORION_ASSERT_DEBUG(_data);
		ORION_ASSERT_DEBUG(new_capacity > 0);
		if (_capacity >= new_capacity) {
			return;
		}

		PointerType new_data
			= static_cast<PointerType>(_allocator.Allocate(sizeof(ValueType) * new_capacity, alignof(ValueType)));
		ORION_ASSERT_DEBUG(new_data);

		if (_size > 0) {
			if constexpr (IsTriviallyCopyable<ValueType>) {
				Platform::MemoryCopy(new_data, _data, ByteSize());
			} else {
				for (SizeType index = 0; index < Size(); ++index) {
					new (&new_data[index]) ValueType(Move(_data[index]));
					_data[index].~ValueType();
				}
			}
		}

		_allocator.Free(_data);
		_data     = new_data;
		_capacity = new_capacity;
	}
}  // namespace Orion::Engine
