#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Algorithms/Compare.h"
#include "Core/Standard/Algorithms/Sort.h"
#include "Core/Standard/Memory/Allocators/Allocator.h"
#include "Core/Standard/Memory/Allocators/PlatformAllocator.h"
#include "Core/Standard/Memory/Lifetime.h"
#include "Core/Standard/Utility/MathUtils.h"
#include "Core/Standard/Utility/MoveAndForward.h"
#include "Core/Standard/Utility/Swap.h"
#include "Platform/Memory.h"

namespace Orion::Engine
{
	/// @brief Verifies that a given element at \p address is NOT already present in the Vector.
#define ORION_VECTOR_VERIFY_ELEMENT_ADDRESS(address)
	ORION_ASSERT_DEBUG_SLOW(address < Data() || address >= (Data() + Size()),
	                        "Attempting to use element (at address '{}'), which is already contained in the Vector.",
	                        address)

	namespace Detail
	{
		/// @brief TODO
		/// @tparam T TODO
		/// @tparam Allocator TODO
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
			static constexpr SizeType k_initial_capacity = 8UL;

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
	}  // namespace Detail

	/**
	 * @brief Represents a type-safe container capable of holding variable amount of elements.
	 * @tparam T Type to be stored.
	 * @tparam Allocator Allocator to be used with the Vector that will perform all the allocations.
	 */
	template <typename T, Memory::AllocatorKind Allocator = Memory::PlatformAllocator>
	class Vector : public Detail::VectorBase<T, Allocator>
	{
		public:
		using BaseType           = Detail::VectorBase<T, Allocator>;
		using ValueType          = BaseType::ValueType;
		using SizeType           = BaseType::SizeType;
		using AllocatorType      = BaseType::AllocatorType;
		using PointerType        = BaseType::PointerType;
		using ConstPointerType   = BaseType::ConstPointerType;
		using ReferenceType      = BaseType::ReferenceType;
		using ConstReferenceType = BaseType::ConstReferenceType;

		static constexpr SizeType k_initial_capacity = BaseType::k_initial_capacity;

		public:
		constexpr explicit Vector(SizeType initial_capacity      = k_initial_capacity,
		                          const AllocatorType& allocator = AllocatorType());
		constexpr explicit Vector(ConstPointerType begin,
		                          ConstPointerType end,
		                          const AllocatorType& allocator = AllocatorType()) noexcept;
		constexpr Vector(std::initializer_list<ValueType>) noexcept;
		constexpr Vector(const Vector&) noexcept;
		constexpr Vector(Vector&&) noexcept;
		constexpr ~Vector();

		constexpr Vector& operator=(const Vector&) noexcept;
		constexpr Vector& operator=(Vector&&) noexcept;
		constexpr Vector& operator=(std::initializer_list<ValueType>) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType operator[](SizeType index) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType operator[](SizeType index) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 operator==(const Vector& other) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 operator!=(const Vector& other) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 operator<(const Vector& other) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 operator<=(const Vector& other) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 operator>(const Vector& other) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 operator>=(const Vector& other) const noexcept;

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
		constexpr void SwapMembers(Vector& other) noexcept;
	};

	// -- Deduction guides.
	template <typename T, Memory::AllocatorKind Allocator = Memory::PlatformAllocator>
	Vector(const T*, const T*, Allocator = Allocator()) -> Vector<T, Allocator>;

	// -- Implementation.
	namespace Detail
	{
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
		constexpr VectorBase<T, Allocator>::VectorBase(SizeType initial_capacity,
		                                               const AllocatorType& allocator) noexcept
			: _allocator(allocator), _data(nullptr), _capacity(0UL), _size(0UL)
		{
			_capacity = ORION_MAX(initial_capacity, k_initial_capacity);
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
			PointerType new_data  = Memory::AllocateCount<ValueType>(this->_allocator, new_capacity);

			Memory::ConstructItems(new_data, _data, _size);
			Memory::DestructItems(_data, _size);

			_allocator.Free(_data);
			_data     = new_data;
			_capacity = new_capacity;
		}
	}  // namespace Detail

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr Vector<T, Allocator>::Vector(SizeType initial_capacity, const AllocatorType& allocator)
		: BaseType(initial_capacity, allocator)
	{
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr Vector<T, Allocator>::Vector(ConstPointerType begin,
	                                       ConstPointerType end,
	                                       const AllocatorType& allocator) noexcept
		: BaseType(static_cast<SizeType>(end - begin), allocator)
	{
		ORION_ASSERT_DEBUG_SLOW(begin);
		ORION_ASSERT_DEBUG_SLOW(end);
		ORION_ASSERT_DEBUG_SLOW(begin <= end);
		SizeType size = static_cast<SizeType>(end - begin);
		Memory::ConstructItems(Data(), begin, size);
		this->_size = size;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr Vector<T, Allocator>::Vector(std::initializer_list<ValueType> list) noexcept
		: BaseType(list.size(), AllocatorType{})
	{
		Memory::ConstructItems(Data(), list.begin(), list.size());
		this->_size = list.size();
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr Vector<T, Allocator>::Vector(const Vector& other) noexcept : BaseType(other.Size(), other.GetAllocator())
	{
		Memory::ConstructItems(Data(), other.Data(), other.Size());
		this->_size = other.Size();
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr Vector<T, Allocator>::Vector(Vector&& other) noexcept : BaseType(Move(other.GetAllocator()))
	{
		SwapMembers(other);
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr Vector<T, Allocator>::~Vector()
	{
		Memory::DestructItems(Data(), Size());
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::operator=(const Vector& other) noexcept -> Vector&
	{
		if (this != &other) {
			Memory::DestructItems(Data(), Size());
			this->EnsureCapacity(other.Size());
			Memory::ConstructItems(Data(), other.Data(), other.Size());
			this->_size = other.Size();
		}
		return *this;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::operator=(Vector&& other) noexcept -> Vector&
	{
		if (this != &other) {
			SwapMembers(other);
		}
		return *this;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::operator=(std::initializer_list<ValueType> list) noexcept -> Vector&
	{
		Memory::DestructItems(Data(), Size());
		this->_size = 0UL;
		this->EnsureCapacity(list.size());
		Memory::ConstructItems(Data(), list.begin(), list.size());
		this->_size = list.size();
		return *this;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::operator[](SizeType index) noexcept -> ReferenceType
	{
		ORION_ASSERT_DEBUG_SLOW(index < _size);
		return this->_data[index];
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::operator[](SizeType index) const noexcept -> ConstReferenceType
	{
		ORION_ASSERT_DEBUG_SLOW(index < _size);
		return this->_data[index];
	}

	template <typename T, Memory::AllocatorKind Allocator>
	ORION_FORCE_INLINE constexpr auto Vector<T, Allocator>::operator==(const Vector& other) const noexcept -> Bool8
	{
		return Algorithm::Compare(Data(), other.Data(), Size(), other.Size()) == 0;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	ORION_FORCE_INLINE constexpr auto Vector<T, Allocator>::operator!=(const Vector& other) const noexcept -> Bool8
	{
		return !(*this == other);
	}

	template <typename T, Memory::AllocatorKind Allocator>
	ORION_FORCE_INLINE constexpr auto Vector<T, Allocator>::operator<(const Vector& other) const noexcept -> Bool8
	{
		return Algorithm::Compare(Data(), other.Data(), Size(), other.Size()) < 0;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	ORION_FORCE_INLINE constexpr auto Vector<T, Allocator>::operator<=(const Vector& other) const noexcept -> Bool8
	{
		return Algorithm::Compare(Data(), other.Data(), Size(), other.Size()) <= 0;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	ORION_FORCE_INLINE constexpr auto Vector<T, Allocator>::operator>(const Vector& other) const noexcept -> Bool8
	{
		return Algorithm::Compare(Data(), other.Data(), Size(), other.Size()) > 0;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	ORION_FORCE_INLINE constexpr auto Vector<T, Allocator>::operator>=(const Vector& other) const noexcept -> Bool8
	{
		return Algorithm::Compare(Data(), other.Data(), Size(), other.Size()) >= 0;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Front() noexcept -> ReferenceType
	{
		return this->_data[0];
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Front() const noexcept -> ConstReferenceType
	{
		return this->_data[0];
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Back() noexcept -> ReferenceType
	{
		return this->_data[this->_size - 1];
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Back() const noexcept -> ConstReferenceType
	{
		return this->_data[this->_size - 1];
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Data() noexcept -> PointerType
	{
		return this->_data;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Data() const noexcept -> ConstPointerType
	{
		return this->_data;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	template <typename... Args>
	constexpr auto Vector<T, Allocator>::AddConstruct(Args&&... args) noexcept -> void
	{
		this->EnsureCapacity(this->_size + 1);
		Memory::ConstructItem(&this->_data[this->_size], Forward<Args>(args)...);
		this->_size += 1;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::AddZeroed(SizeType count) noexcept -> void
	{
		this->EnsureCapacity(this->_size + count);
		Memory::DefaultConstructItems<ValueType>(&this->_data[this->_size], count);
		this->_size += count;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Add(const ValueType& value) noexcept -> void
	{
		ORION_VECTOR_VERIFY_ELEMENT_ADDRESS(&value);

		this->EnsureCapacity(this->_size + 1);
		Memory::ConstructItem(&this->_data[this->_size], value);
		this->_size += 1;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Add(ValueType&& value) noexcept -> void
	{
		ORION_VECTOR_VERIFY_ELEMENT_ADDRESS(&value);

		this->EnsureCapacity(this->_size + 1);
		Memory::ConstructItem(&this->_data[this->_size], Move(value));
		this->_size += 1;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::AddRange(ConstPointerType begin, ConstPointerType end) -> void
	{
		ORION_ASSERT_DEBUG_SLOW(begin);
		ORION_ASSERT_DEBUG_SLOW(end);
		ORION_ASSERT_DEBUG_SLOW(begin <= end);
		ORION_VECTOR_VERIFY_ELEMENT_ADDRESS(begin);
		ORION_VECTOR_VERIFY_ELEMENT_ADDRESS(end);

		SizeType size = static_cast<SizeType>(end - begin);
		this->EnsureCapacity(Size() + size);
		Memory::ConstructItems(&this->_data[this->_size], begin, size);
		this->_size += size;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::RemoveBack() noexcept -> void
	{
		ORION_ASSERT_DEBUG_SLOW(_data);
		ORION_ASSERT_DEBUG_SLOW(_size > 0);
		this->_data[--this->_size].~ValueType();
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Remove(SizeType index) noexcept -> void
	{
		ORION_ASSERT_DEBUG_SLOW(_data);
		ORION_ASSERT_DEBUG_SLOW(_size > 0);
		Swap(this->_data[index], this->_data[this->_size - 1]);
		this->_data[--this->_size].~ValueType();
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Reserve(SizeType size) noexcept -> void
	{
		this->EnsureCapacity(size);
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Clear() noexcept -> void
	{
		Memory::DestructItems(this->_data, this->_size);
		this->_size = 0;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	template <typename CompareFn>
	constexpr auto Vector<T, Allocator>::Sort(CompareFn&& compare) noexcept -> void
	{
		ORION_ASSERT_DEBUG_SLOW(_data != nullptr);
		Algorithm::Sort(this->_data, this->_size, Forward<CompareFn>(compare));
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::IsEmpty() const noexcept -> Bool8
	{
		return this->_size == 0;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Size() const noexcept -> SizeType
	{
		return this->_size;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::ByteSize() const noexcept -> SizeType
	{
		return sizeof(ValueType) * Size();
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::Capacity() const noexcept -> SizeType
	{
		return this->_capacity;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::begin() noexcept -> PointerType
	{
		return this->_data;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::begin() const noexcept -> ConstPointerType
	{
		return this->_data;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::end() noexcept -> PointerType
	{
		return this->_data + this->_size;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::end() const noexcept -> ConstPointerType
	{
		return this->_data + this->_size;
	}

	template <typename T, Memory::AllocatorKind Allocator>
	constexpr auto Vector<T, Allocator>::SwapMembers(Vector& other) noexcept -> void
	{
		Swap(this->_allocator, other._allocator);
		Swap(this->_data, other._data);
		Swap(this->_capacity, other._capacity);
		Swap(this->_size, other._size);
	}
}  // namespace Orion::Engine

#undef ORION_VECTOR_VERIFY_ELEMENT_ADDRESS
