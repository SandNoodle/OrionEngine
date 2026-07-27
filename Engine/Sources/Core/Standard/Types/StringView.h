#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Algorithms/Hash.h"
#include "Core/Standard/Types/Detail/StringFwd.h"
#include "Core/Standard/Utility/StringUtils.h"
#include "Platform/Memory.h"

namespace Orion::Engine
{
	namespace Detail
	{
		/**
		 * @brief TODO
		 * @tparam T
		 */
		template <StringEncoding T>
		class StringViewBase
		{
			public:
			using ThisType           = StringViewBase;
			using ValueType          = StringTraits<T>::CharType;
			using SizeType           = USize;
			using PointerType        = ValueType*;
			using ConstPointerType   = const ValueType*;
			using ReferenceType      = ValueType&;
			using ConstReferenceType = const ValueType&;
			using HashType           = UInt64;

			private:
			ConstPointerType _data{ nullptr };
			SizeType _size{ 0UL };

			public:
			constexpr explicit StringViewBase() = default;
			constexpr explicit StringViewBase(ConstPointerType data, SizeType size) noexcept;

			[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType operator[](SizeType index) noexcept;
			[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType operator[](SizeType index) const noexcept;

			[[nodiscard]] static constexpr Bool8 Equal(ThisType lhs, ThisType rhs) noexcept;

			[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Front() noexcept;
			[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType Front() const noexcept;
			[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Back() noexcept;
			[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType Back() const noexcept;
			[[nodiscard]] ORION_FORCE_INLINE constexpr PointerType Data() noexcept;
			[[nodiscard]] ORION_FORCE_INLINE constexpr ConstPointerType Data() const noexcept;

			/// @brief Returns the Hash value of the StringView.
			[[nodiscard]] constexpr HashType Hash() const noexcept;

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
	}  // namespace Detail

	// -- Type aliases.
	using StringView      = Detail::StringViewBase<Detail::StringEncoding::ANSI>;
	using StringViewUTF8  = Detail::StringViewBase<Detail::StringEncoding::UTF8>;
	using StringViewUTF16 = Detail::StringViewBase<Detail::StringEncoding::UTF16>;
	using StringViewUTF32 = Detail::StringViewBase<Detail::StringEncoding::UTF32>;

	// -- Deduction guides.
	// TODO(SandNoodle): Deduction guides.

	// -- Helper macros.
	/// @brief Constructs StringView from a C-styled literal.
#define ORION_STRINGVIEW(str) \
	StringView(reinterpret_cast<StringView::ConstPointerType>(str), StringLength<Detail::StringEncoding::ANSI>(str))

	// -- Hash.
	template <>
	struct Hash<StringView>
	{
		public:
		using ValueType = StringView;
		using SizeType  = USize;

		public:
		SizeType operator()(const ValueType& v)
		{
			return v.Hash();
		}
	};

	// -- Implementation.
	namespace Detail
	{
		template <StringEncoding T>
		constexpr StringViewBase<T>::StringViewBase(ConstPointerType data, SizeType size) noexcept
			: _data(data), _size(size)
		{
		}

		template <StringEncoding T>
		constexpr auto StringViewBase<T>::Equal(ThisType lhs, ThisType rhs) noexcept -> Bool8
		{
			if (lhs.Size() != rhs.Size()) {
				return false;
			}
			return Platform::MemoryCompare(lhs._data, rhs._data, lhs.ByteSize()) == 0;
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::Front() noexcept -> ReferenceType
		{
			return *_data;
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::Front() const noexcept -> ConstReferenceType
		{
			return *_data;
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::Back() noexcept -> ReferenceType
		{
			return *_data[_size - 1];
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::Back() const noexcept -> ConstReferenceType
		{
			return *_data[_size - 1];
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::Data() noexcept -> PointerType
		{
			return _data;
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::Data() const noexcept -> ConstPointerType
		{
			return _data;
		}

		template <StringEncoding T>
		constexpr auto StringViewBase<T>::Hash() const noexcept -> HashType
		{
			return FNV1AHash(_data, _size);
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::IsEmpty() const noexcept -> Bool8
		{
			return _size == 0;
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::Size() const noexcept -> SizeType
		{
			return _size;
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::ByteSize() const noexcept -> SizeType
		{
			return _size * sizeof(ValueType);
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::operator[](SizeType index) noexcept -> ReferenceType
		{
			ORION_ASSERT_DEBUG(index < _size);
			return _data[index];
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::operator[](SizeType index) const noexcept
			-> ConstReferenceType
		{
			ORION_ASSERT_DEBUG(index < _size);
			return _data[index];
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::begin() noexcept -> PointerType
		{
			return &_data[0];
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::begin() const noexcept -> ConstPointerType
		{
			return &_data[0];
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::end() noexcept -> PointerType
		{
			return &_data[_size - 1];
		}

		template <StringEncoding T>
		ORION_FORCE_INLINE constexpr auto StringViewBase<T>::end() const noexcept -> ConstPointerType
		{
			return &_data[_size - 1];
		}
	}  // namespace Detail
}  // namespace Orion::Engine
