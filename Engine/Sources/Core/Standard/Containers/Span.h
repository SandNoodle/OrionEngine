#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Concepts.h"
#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Limits.h"

namespace Orion::Engine
{
	/// @brief Concept that is satisfied by types that allow continuous access to their underlying data.
	template <typename ContainerT>
	concept SpannableKind = requires(ContainerT container) {
		{ container.Size() } -> SameAs<typename ContainerT::SizeType>;
		{ container.Data() } -> SameAs<typename ContainerT::PointerType>;
	};

	/// @brief Describes that the amount of elements covered by a given Span is only known at runtime.
	static constexpr USize k_dynamic_extent = NumericLimits<USize>::Max();

	/// @brief Represents type-safe linear access to a sequence of elements.
	/// @tparam T Type to be accessed.
	/// @tparam Extent Describes the amount of elements known at compile time; this parameter is set to k_dynamic_extent
	/// if they are only known at runtime.
	template <typename T, USize Extent = k_dynamic_extent>
	class Span
	{
		public:
		using ValueType          = T;
		using SizeType           = USize;
		using PointerType        = T*;
		using ConstPointerType   = const T*;
		using ReferenceType      = T&;
		using ConstReferenceType = const T&;

		static constexpr Bool8 k_is_dynamic = Extent == k_dynamic_extent;
		static constexpr Bool8 k_is_static  = Extent != k_dynamic_extent;

		private:
		PointerType _data{ nullptr };
		SizeType _size{ 0UL };

		public:
		constexpr Span() noexcept = default;

		constexpr explicit Span(PointerType values, SizeType size) noexcept;

		template <typename U, USize N>
		constexpr explicit Span(Array<U, N>& array) noexcept;

		template <typename U, USize N>
		constexpr explicit Span(const Array<U, N>& array) noexcept;

		template <SpannableKind Spannable>
		constexpr explicit Span(Spannable& s) noexcept;

		template <SpannableKind Spannable>
		constexpr explicit Span(const Spannable& s) noexcept;

		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType operator[](SizeType index) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType operator[](SizeType index) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr bool operator==(const Span& other) const noexcept;

		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Front() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType Front() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Back() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType Back() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr PointerType Data() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstPointerType Data() const noexcept;

		/// @brief Verifies if the container is empty, i.e. points to 0 elements.
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 IsEmpty() const noexcept;

		/// @brief Returns the amount of elements that this container points to.
		[[nodiscard]] ORION_FORCE_INLINE constexpr SizeType Size() const noexcept;

		public:
		// NOLINTBEGIN(readability-identifier-naming)
		/// @brief Required overload for the C++'s for range loops.
		///@{
		[[nodiscard]] ORION_FORCE_INLINE constexpr PointerType begin() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstPointerType begin() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr PointerType end() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstPointerType end() const noexcept;
		///@}
		// NOLINTEND(readability-identifier-naming)
	};

	// -- Aliases.
	template <typename T, USize Extent = k_dynamic_extent>
	using ReadonlySpan = Span<const T, Extent>;

	// -- Deduction guides.
	template <class T>
	Span(T*, USize) -> Span<T>;

	template <class T>
	Span(const T*, USize) -> Span<const T>;

	template <class T, USize N>
	Span(T (&)[N]) -> Span<T, N>;

	template <class T, USize N>
	Span(Array<T, N>&) -> Span<T, N>;

	template <class T, USize N>
	Span(const Array<T, N>&) -> Span<const T, N>;

	template <SpannableKind Spannable>
	Span(Spannable&) -> Span<typename Spannable::ValueType>;

	template <SpannableKind Spannable>
	Span(const Spannable&) -> Span<const typename Spannable::ValueType>;

	// -- Implementation.
	template <typename T, USize Extent>
	constexpr Span<T, Extent>::Span(PointerType values, SizeType size) noexcept : _data(values), _size(size)
	{
	}

	template <typename T, USize Extent>
	template <typename U, USize N>
	constexpr Span<T, Extent>::Span(Array<U, N>& array) noexcept : _data(array.Data()), _size(array.Size())
	{
	}

	template <typename T, USize Extent>
	template <typename U, USize N>
	constexpr Span<T, Extent>::Span(const Array<U, N>& array) noexcept : _data(array.Data()), _size(array.Size())
	{
	}

	template <typename T, USize Extent>
	template <SpannableKind Spannable>
	constexpr Span<T, Extent>::Span(Spannable& s) noexcept : _data(s.Data()), _size(s.Size())
	{
	}

	template <typename T, USize Extent>
	template <SpannableKind Spannable>
	constexpr Span<T, Extent>::Span(const Spannable& s) noexcept : _data(s.Data()), _size(s.Size())
	{
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::operator[](SizeType index) noexcept -> ReferenceType
	{
		ORION_ASSERT_DEBUG(index < _size);
		return _data[index];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::operator[](SizeType index) const noexcept -> ConstReferenceType
	{
		ORION_ASSERT_DEBUG(index < _size);
		return _data[index];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::operator==(const Span& other) const noexcept -> bool
	{
		if (Size() != other.Size()) {
			return false;
		}

		if (Size() == 0) [[unlikely]] {
			return true;
		}

		for (SizeType index = 0; index < Size(); ++index) {
			if (_data[index] != other[index]) {
				return false;
			}
		}

		return true;
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::IsEmpty() const noexcept -> Bool8
	{
		return _size == 0;
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::Size() const noexcept -> SizeType
	{
		return _size;
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::Front() noexcept -> ReferenceType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return _data[0];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::Front() const noexcept -> ConstReferenceType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return _data[0];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::Back() noexcept -> ReferenceType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return _data[_size - 1];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::Back() const noexcept -> ConstReferenceType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return _data[_size - 1];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::Data() noexcept -> PointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return &_data[0];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::Data() const noexcept -> ConstPointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return &_data[0];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::begin() noexcept -> PointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return _data;
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::begin() const noexcept -> ConstPointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return _data;
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::end() noexcept -> PointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return _data + _size;
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::end() const noexcept -> ConstPointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return _data + _size;
	}
}  // namespace Orion::Engine
