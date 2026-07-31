#pragma once

#include "OrionEngine.h"

#include "Core/Memory/Allocators/PlatformAllocator.h"
#include "Core/Standard/Algorithms/Hash.h"
#include "Core/Standard/Containers/Detail/StringFwd.h"
#include "Core/Standard/Containers/Span.h"
#include "Core/Standard/Containers/Vector.h"
#include "Core/Standard/Utility/StringUtils.h"
#include "StringView.h"

namespace Orion::Engine
{
	namespace Detail
	{
		/**
		 * @brief String is a class that stores and allows manipulation of continuous sequence of characters in a given
		 * encoding.
		 * @tparam T Encoding used for storing the characters.
		 * @tparam Allocator Allocator to be used with the string that will perform all the allocations.
		 */
		template <StringEncoding T, typename Allocator = PlatformAllocator>
		class StringBase : Vector<typename StringTraits<T>::CharType, Allocator>
		{
			public:
			using ThisType           = StringBase;
			using TraitType          = StringTraits<T>;
			using CharType           = TraitType::CharType;
			using WideCharType       = TraitType::WideCharType;
			using BaseType           = Vector<CharType, Allocator>;
			using ValueType          = BaseType::ValueType;
			using SizeType           = BaseType::SizeType;
			using AllocatorType      = BaseType::AllocatorType;
			using PointerType        = BaseType::PointerType;
			using ConstPointerType   = BaseType::ConstPointerType;
			using ReferenceType      = BaseType::ReferenceType;
			using ConstReferenceType = BaseType::ConstReferenceType;
			using HashType           = Algorithm::Hash<CharType>::SizeType;

			public:
			constexpr explicit StringBase() noexcept = default;
			constexpr explicit StringBase(CString str) noexcept;
			constexpr explicit StringBase(ConstPointerType data, SizeType size) noexcept;

			/// @brief Appends single \p character to the end of the string.
			constexpr ThisType& Append(WideCharType character) noexcept;

			/**
			 * @brief Appends C-styled string literal to the end of the string.
			 * @warning Assumes that the C-String is in the correct encoding.
			 */
			constexpr ThisType& Append(CString str) noexcept;

			/// @brief Appends Span of a given characters to the end of the string.
			constexpr ThisType& Append(ReadonlySpan<CharType> view) noexcept;

			/// @brief Appends StringView of a given encoding to the end of the string.
			constexpr ThisType& Append(StringViewBase<T> view) noexcept;

			/// @brief Appends some string range between \p begin and \p to end of the string.
			constexpr ThisType& AppendRange(ConstPointerType begin, ConstPointerType end) noexcept;

			/**
			 * @brief Returns character from the string at a given index.
			 * @details This is a correct method of acquiring a valid UTF-8 encoded character.
			 * @warning \p index must be less than Length().
			 * @param index Index of the character.
			 */
			[[nodiscard]] constexpr WideCharType GetChar(SizeType index) const noexcept;

			/**
			 * @brief Returns the number of characters making up this string.
			 * @warning For variable-length encodings (such as UTF-8) it might not return a valid character count.
			 * Instead it the number of 'bytes' (i.e. CharType) will be returned that make up the string. To get the
			 * accurate character count use Length().
			 */
			[[nodiscard]] ORION_FORCE_INLINE constexpr SizeType Size() const noexcept;

			/**
			 * @brief Returns the number of characters making up this string.
			 * @important This is a correct function to acquire the number of actual encoded characters making up the
			 * string for variable-length encodings (such as UTF-8).
			 */
			[[nodiscard]] ORION_FORCE_INLINE constexpr SizeType Length() const noexcept;

			/// @brief Returns the Hash value of the StringView.
			[[nodiscard]] constexpr HashType Hash() const noexcept;

			public:
			using BaseType::operator=;
			using BaseType::operator[];
			using BaseType::Back;
			using BaseType::ByteSize;
			using BaseType::Capacity;
			using BaseType::Data;
			using BaseType::Front;
			using BaseType::IsEmpty;
			using BaseType::Reserve;

			// NOLINTBEGIN(readability-identifier-naming)
			/** Required overload for the C++'s for range loops. */
			///@{
			using BaseType::begin;
			using BaseType::end;
			///@}
			// NOLINTEND(readability-identifier-naming)
		};
	}  // namespace Detail

	// -- Type aliases.
	using String      = Detail::StringBase<Detail::StringEncoding::ANSI>;
	using StringUTF8  = Detail::StringBase<Detail::StringEncoding::UTF8>;
	using StringUTF16 = Detail::StringBase<Detail::StringEncoding::UTF16>;
	using StringUTF32 = Detail::StringBase<Detail::StringEncoding::UTF32>;

	// -- Helper macros.
	/// @brief Constructs String from a C-Styled literal (\p str).
#define ORION_STRING(str) \
	String(reinterpret_cast<String::ConstPointerType>(str), StringLength<Detail::StringEncoding::ANSI>(str))

	// -- Hash.
	namespace Algorithm
	{
#define ORION_STRING_HASH(type)                       \
	template <>                                       \
	struct Hash<type>                                 \
	{                                                 \
		public:                                       \
		using ValueType = type;                       \
		using SizeType  = USize;                      \
                                                      \
		public:                                       \
		SizeType operator()(const ValueType& v) const \
		{                                             \
			return v.Hash();                          \
		}                                             \
	};

		ORION_STRING_HASH(String);
		ORION_STRING_HASH(StringUTF8);
		ORION_STRING_HASH(StringUTF16);
		ORION_STRING_HASH(StringUTF32);
#undef ORION_STRING_HASH
	}  // namespace Algorithm

	// -- Implementation.
	template <Detail::StringEncoding T, typename Allocator>
	constexpr Detail::StringBase<T, Allocator>::StringBase(CString str) noexcept
	{
		SizeType size = StringLength<T>(str);
		BaseType::AddZeroed(size);
		for (SizeType index = 0; index < size; ++index) {
			Data()[index] = str[index];
		}
	}

	template <Detail::StringEncoding T, typename Allocator>
	constexpr Detail::StringBase<T, Allocator>::StringBase(ConstPointerType data, SizeType size) noexcept
	{
		ORION_ASSERT_DEBUG(data);
		BaseType::AddZeroed(size);
		for (SizeType index = 0; index < size; ++index) {
			Data()[index] = data[index];
		}
	}

	template <Detail::StringEncoding T, typename Allocator>
	constexpr auto Detail::StringBase<T, Allocator>::Append(WideCharType character) noexcept -> ThisType&
	{
		if constexpr (IsSame<CharType, WideCharType>) {
			this->Add(character);
		} else {
			CharType buffer[sizeof(WideCharType)];
			SizeType bytes_to_copy = sizeof(CharType) * UTF8CodepointsForCharacter(character);
			Platform::MemoryCopy(buffer, &character, bytes_to_copy);
			return AppendRange(buffer, buffer + bytes_to_copy);
		}
		return *this;
	}

	template <Detail::StringEncoding T, typename Allocator>
	constexpr auto Detail::StringBase<T, Allocator>::Append(CString str) noexcept -> ThisType&
	{
		SizeType str_size       = StringLength<T>(str);
		SizeType current_offset = BaseType::Size();
		this->AddZeroed(str_size);
		for (SizeType index = 0; index < str_size; ++index) {
			BaseType::Data()[current_offset + index] = static_cast<CharType>(str[index]);
		}
		return *this;
	}

	template <Detail::StringEncoding T, typename Allocator>
	constexpr auto Detail::StringBase<T, Allocator>::Append(ReadonlySpan<CharType> view) noexcept -> ThisType&
	{
		return AppendRange(view.begin(), view.end());
	}

	template <Detail::StringEncoding T, typename Allocator>
	constexpr auto Detail::StringBase<T, Allocator>::Append(StringViewBase<T> view) noexcept -> ThisType&
	{
		return AppendRange(view.begin(), view.end());
	}

	template <Detail::StringEncoding T, typename Allocator>
	constexpr auto Detail::StringBase<T, Allocator>::AppendRange(ConstPointerType begin, ConstPointerType end) noexcept
		-> ThisType&
	{
		ORION_ASSERT_DEBUG(begin);
		ORION_ASSERT_DEBUG(end);
		ORION_ASSERT_DEBUG(begin <= end);
		this->AddRange(begin, end);
		return *this;
	}

	template <Detail::StringEncoding T, typename Allocator>
	constexpr auto Detail::StringBase<T, Allocator>::GetChar(SizeType index) const noexcept -> WideCharType
	{
		ORION_ASSERT_DEBUG(index < Length());
		if constexpr (IsSame<CharType, WideCharType>) {
			return BaseType::operator[](index);
		}

		if constexpr (T == StringEncoding::UTF8) {
			CharType c = BaseType::operator[](index);
			if (c >= 0 && c <= 127) {
				return c;
			}

			WideCharType result;
			SizeType bytes_to_copy = UTF8CodepointsForCharacter(c) * sizeof(CharType);
			Platform::MemoryCopy(&result, BaseType::Data() + index, bytes_to_copy);
			return result;
		} else {
			ORION_NOT_IMPLEMENTED("Unknown String encoding.");
		}
	}

	template <Detail::StringEncoding T, typename Allocator>
	constexpr auto Detail::StringBase<T, Allocator>::Size() const noexcept -> SizeType
	{
		return BaseType::Size();
	}

	template <Detail::StringEncoding T, typename Allocator>
	constexpr auto Detail::StringBase<T, Allocator>::Length() const noexcept -> SizeType
	{
		return StringLength<T>(BaseType::begin(), BaseType::end());
	}

	template <Detail::StringEncoding T, typename Allocator>
	constexpr auto Detail::StringBase<T, Allocator>::Hash() const noexcept -> HashType
	{
		return FNV1AHash<CharType, HashType>(Data(), Size());
	}
}  // namespace Orion::Engine
