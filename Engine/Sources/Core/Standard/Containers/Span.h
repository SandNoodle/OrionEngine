#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Concepts.h"
#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Limits.h"
#include "Core/Standard/TypeTraits.h"

namespace Orion::Engine
{
	template <typename ContainerT>
	concept SpannableKind = requires(ContainerT container) {
		{ container.Size() } -> SameAs<typename ContainerT::SizeType>;
		{ container.Data() } -> SameAs<typename ContainerT::PointerType>;
	};

	static constexpr USize k_dynamic_extent = NumericLimits<USize>::Max();

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

		private:
		PointerType _data{ nullptr };
		SizeType _size{ 0 };

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

		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Front() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType Front() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Back() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType Back() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr PointerType Data() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstPointerType Data() const noexcept;

		[[nodiscard]] ORION_FORCE_INLINE constexpr PointerType Begin() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstPointerType Begin() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr PointerType End() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstPointerType End() const noexcept;

		/// Verifies if the container is empty, i.e. points to 0 elements.
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 IsEmpty() const noexcept;

		/// Returns the amount of elements that this container points to.
		[[nodiscard]] ORION_FORCE_INLINE constexpr SizeType Size() const noexcept;

		public:
		// NOLINTBEGIN(readability-identifier-naming)
		/** Required overload for the C++'s for range loops. Prefer using uppercase versions. */
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
	constexpr auto Span<T, Extent>::Begin() noexcept -> PointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return &_data[0];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::Begin() const noexcept -> ConstPointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return &_data[0];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::End() noexcept -> PointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return &_data[_size - 1];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::End() const noexcept -> ConstPointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return &_data[_size - 1];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::begin() noexcept -> PointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return &_data[0];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::begin() const noexcept -> ConstPointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return &_data[0];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::end() noexcept -> PointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return &_data[_size - 1];
	}

	template <typename T, USize Extent>
	constexpr auto Span<T, Extent>::end() const noexcept -> ConstPointerType
	{
		ORION_ASSERT_DEBUG(_size > 0);
		return &_data[_size - 1];
	}
}  // namespace Orion::Engine
