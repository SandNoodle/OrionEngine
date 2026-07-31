#pragma once

#include "OrionEngine.h"

namespace Orion::Engine::Detail
{
	/// @brief Represents encoding used when storing the String type.
	enum class StringEncoding : UInt8
	{
		ANSI,
		UTF8,
		UTF16,
		UTF32,
	};

	/// @brief Represents common traits for a given String encoding.
	template <StringEncoding>
	struct StringTraits
	{
	};

	// -- Type forwarding.
	template <StringEncoding T, typename Allocator>
	class StringBase;

	template <StringEncoding T>
	class StringViewBase;

	// -- Template specializations.
	template <>
	struct StringTraits<StringEncoding::ANSI>
	{
		static constexpr StringEncoding k_encoding = StringEncoding::ANSI;

		using CharType     = Char8;
		using WideCharType = Char8;
	};

	template <>
	struct StringTraits<StringEncoding::UTF8>
	{
		static constexpr StringEncoding k_encoding = StringEncoding::UTF8;

		using CharType     = Char8;
		using WideCharType = Char32;
	};

	template <>
	struct StringTraits<StringEncoding::UTF16>
	{
		static constexpr StringEncoding k_encoding = StringEncoding::UTF16;

		using CharType     = Char16;
		using WideCharType = Char16;
	};

	template <>
	struct StringTraits<StringEncoding::UTF32>
	{
		static constexpr StringEncoding k_encoding = StringEncoding::UTF32;

		using CharType     = Char32;
		using WideCharType = Char32;
	};
}  // namespace Orion::Engine::Detail
