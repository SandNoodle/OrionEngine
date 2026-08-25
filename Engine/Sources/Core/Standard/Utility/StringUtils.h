#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Containers/Detail/StringFwd.h"
#include "Core/Standard/Limits.h"

namespace Orion::Engine
{
	/**
	 * @brief Calculates the number of codepoints (bytes) required to store given UTF-8 encoded character.
	 * @warning Returns 0 if the character cannot be represented (i.e. an error occurred).
	 * @tparam SizeType Return type.
	 * @tparam CharType Type of the UTF-8 character.
	 * @param c Character for which calculate the number of codepoints required.
	 */
	template <typename SizeType = USize,
	          typename CharType = Detail::StringTraits<Detail::StringEncoding::UTF8>::CharType>
	[[nodiscard]] constexpr SizeType UTF8CodepointsForCharacter(CharType c) noexcept
	{
		if (c >= 0 && c <= 127) {
			return 1;
		}

		if ((c & 0xE0) == 0xC0) {
			return 2;
		}

		if ((c & 0xF0) == 0xE0) {
			return 3;
		}

		if ((c & 0xF8) == 0xF0) {
			return 4;
		}

		return 0;
	}

	/**
	 * @brief Calculates the (character) length of null terminated (`\0`) string literal.
	 * @warning Returns 0, if an error occurs.
	 * @tparam Encoding (Assumed) encoding of the string literal.
	 * @tparam SizeType Return type.
	 * @tparam CharType Type of a single character.
	 * @param str String literal for which the number of characters will be calculated.
	 * @param max_length Count of characters to end the iteration on if null terminator is not present.
	 * @return Return the number of characters present in a given string literal.
	 */
	template <Detail::StringEncoding Encoding,
	          typename SizeType = USize,
	          typename CharType = Detail::StringTraits<Encoding>::CharType>
	[[nodiscard]] constexpr SizeType StringLength(const CharType* str,
	                                              SizeType max_length = NumericLimits<SizeType>::Max()) noexcept
	{
		ORION_ASSERT_DEBUG(str);

		// Some string encodings (such as UTF-8) might encode a single character as a varied number of bytes.
		if constexpr (Encoding == Detail::StringEncoding::UTF8) {
			SizeType length    = 0UL;
			SizeType codepoint = 0UL;
			for (; length < max_length; ++length) {
				CharType c = str[codepoint];
				if (c == '\0') {
					return length;
				}
				codepoint += UTF8CodepointsForCharacter(c);
			}
			return length;
		} else {
			SizeType length = 0UL;
			while (str[length] != '\0' && length < max_length) {
				++length;
			}
			return length;
		}
	}

	/**
	 * @brief Calculates the (character) length of a given string (from a range).
	 * @tparam Encoding (Assumed) encoding of the string.
	 * @tparam SizeType Return type.
	 * @tparam CharType Type of a single character.
	 * @param begin Start of the string range.
	 * @param end End of the string range.
	 */
	template <Detail::StringEncoding Encoding,
	          typename SizeType = USize,
	          typename CharType = Detail::StringTraits<Encoding>::CharType>
	[[nodiscard]] constexpr SizeType StringLength(const CharType* begin, const CharType* end) noexcept
	{
		ORION_ASSERT_DEBUG(begin);
		ORION_ASSERT_DEBUG(end);
		ORION_ASSERT_DEBUG(begin <= end);

		SizeType size = static_cast<SizeType>(end - begin);
		if constexpr (Encoding == Detail::StringEncoding::UTF8) {
			SizeType length    = 0UL;
			SizeType codepoint = 0UL;
			for (; length < size; ++length) {
				CharType c = begin[codepoint];
				codepoint += UTF8CodepointsForCharacter(c);
			}
			return length;
		} else {
			return size;
		}
	}

	/// @brief Hashes given sized-string into a FNV-1a Hash
	template <typename CharT, typename SizeType = USize>
	[[nodiscard]] constexpr SizeType FNV1AHash(const CharT* data, SizeType size) noexcept
	{
		SizeType hash = 0xCBF29CE484222325;
		for (USize index = 0; index < size; ++index) {
			hash ^= static_cast<SizeType>(data[index]);
			hash *= 0x00000100000001B3;
		}
		return hash;
	}
}  // namespace Orion::Engine
