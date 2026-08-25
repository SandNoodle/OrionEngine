#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Algorithms/Sort.h"
#include "Core/Standard/Utility/MoveAndForward.h"

namespace Orion::Engine
{
	/**
	 * @brief Represents type-safe linear container of a fixed type.
	 * In reality, it's just a thin wrapper around C-styled arrays that does not decay to a pointer and contains some
	 * generic methods for convenience.
	 * @tparam T Type to be stored.
	 * @tparam N Maximum amount of elements that can be stored.
	 */
	template <typename T, USize N>
	struct Array
	{
		public:
		using ValueType          = T;
		using SizeType           = USize;
		using PointerType        = ValueType*;
		using ConstPointerType   = const ValueType*;
		using ReferenceType      = ValueType&;
		using ConstReferenceType = const ValueType&;

		public:
		ValueType _data[N];

		public:
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType operator[](SizeType index) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType operator[](SizeType index) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr bool operator==(const Array& other) const noexcept;

		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Front() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType Front() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Back() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType Back() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr PointerType Data() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstPointerType Data() const noexcept;

		/// @brief Overrides the contents of the container with a given \p value.
		ORION_FORCE_INLINE constexpr void Fill(ConstReferenceType value) noexcept;

		/// @brief Sorts the container using a given \p compare predicate function.
		template <typename CompareFn>
		constexpr void Sort(CompareFn&& compare) noexcept;

		/// @brief Verifies if the container is empty, i.e. can store only 0 elements.
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 IsEmpty() const noexcept;

		/// @brief Returns the amount of elements that can be stored in this container.
		[[nodiscard]] ORION_FORCE_INLINE constexpr SizeType Size() const noexcept;

		/// @brief Returns the amount of bytes that this container stores.
		[[nodiscard]] ORION_FORCE_INLINE constexpr SizeType ByteSize() const noexcept;

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
	};

	// -- Deduction guides.
	template <typename T, class... U>
	Array(T, U...) -> Array<T, sizeof...(U) + 1>;

	// -- Implementation.
	template <typename T, USize N>
	constexpr auto Array<T, N>::operator[](SizeType index) noexcept -> ReferenceType
	{
		ORION_ASSERT_DEBUG_SLOW(index < N);
		return _data[index];
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::operator[](SizeType index) const noexcept -> ConstReferenceType
	{
		ORION_ASSERT_DEBUG_SLOW(index < N);
		return _data[index];
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::operator==(const Array& other) const noexcept -> bool
	{
		if (N != other.Size()) {
			return false;
		}

		if (N == 0) [[unlikely]] {
			return true;
		}

		for (SizeType index = 0; index < N; ++index) {
			if (_data[index] != other[index]) {
				return false;
			}
		}
		return true;
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::Fill(ConstReferenceType value) noexcept -> void
	{
		for (SizeType index = 0; index < N; ++index) {
			_data[index] = value;
		}
	}

	template <typename T, USize N>
	template <typename CompareFn>
	constexpr auto Array<T, N>::Sort(CompareFn&& compare) noexcept -> void
	{
		Algorithm::Sort(this->_data, N, Forward<CompareFn>(compare));
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::IsEmpty() const noexcept -> Bool8
	{
		return N == 0;
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::Size() const noexcept -> USize
	{
		return N;
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::ByteSize() const noexcept -> USize
	{
		return sizeof(ValueType) * N;
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::Front() noexcept -> ReferenceType
	{
		return _data[0];
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::Front() const noexcept -> ConstReferenceType
	{
		return _data[0];
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::Back() noexcept -> ReferenceType
	{
		return _data[N - 1];
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::Back() const noexcept -> ConstReferenceType
	{
		return _data[N - 1];
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::Data() noexcept -> PointerType
	{
		ORION_ASSERT_DEBUG_SLOW(N != 0);
		return _data;
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::Data() const noexcept -> ConstPointerType
	{
		ORION_ASSERT_DEBUG_SLOW(N != 0);
		return _data;
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::begin() noexcept -> PointerType
	{
		return _data;
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::begin() const noexcept -> ConstPointerType
	{
		return _data;
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::end() noexcept -> PointerType
	{
		ORION_ASSERT_DEBUG_SLOW(N != 0);
		return _data + N;
	}

	template <typename T, USize N>
	constexpr auto Array<T, N>::end() const noexcept -> ConstPointerType
	{
		ORION_ASSERT_DEBUG_SLOW(N != 0);
		return _data + N;
	}
}  // namespace Orion::Engine
