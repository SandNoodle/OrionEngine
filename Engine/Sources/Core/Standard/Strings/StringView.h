#pragma once

#include "OrionEngine.h"

namespace Orion::Engine
{
	namespace Detail
	{
		template <typename CharT>
		class StringViewBase
		{
			public:
			using ThisType         = StringViewBase<CharT>;
			using ValueType        = CharT;
			using SizeType         = USize;
			using PointerType      = ValueType*;
			using ConstPointerType = const ValueType*;
			using HashType         = UInt64;

			private:
			ConstPointerType _data{ nullptr };
			SizeType _size{ 0UL };

			public:
			constexpr explicit StringViewBase() = default;
			constexpr explicit StringViewBase(ConstPointerType data, SizeType size) noexcept;
			constexpr explicit StringViewBase(const char* data, SizeType size) noexcept;

			/// @brief Constructs a new StringView from a C-style string literal.
			[[nodiscard]] ORION_FORCE_INLINE static constexpr StringViewBase FromStringLiteral(const char*) noexcept;

			[[nodiscard]] constexpr HashType Hash() const noexcept;
		};
	}  // namespace Detail

	using StringView = Detail::StringViewBase<Char8>;

	// -- Implementation.
	namespace Detail
	{
		template <typename CharT>
		constexpr StringViewBase<CharT>::StringViewBase(ConstPointerType data, SizeType size) noexcept
			: _data(data), _size(size)
		{
		}

		template <typename CharT>
		constexpr StringViewBase<CharT>::StringViewBase(const char* data, SizeType size) noexcept
			: _data(data), _size(size)
		{
		}

		template <typename CharT>
		constexpr auto StringViewBase<CharT>::FromStringLiteral(const char* literal) noexcept -> ThisType
		{
			if (literal == nullptr) [[unlikely]] {
				return ThisType{};
			}

			SizeType length = 0UL;
			while (literal[length++] != '\0') {
				// Explicitly nothing.
			}

			return ThisType(reinterpret_cast<ConstPointerType>(literal), length);
		}

		template <typename CharT>
		constexpr auto StringViewBase<CharT>::Hash() const noexcept -> HashType
		{
			HashType hash = 0xCBF29CE484222325;
			for (SizeType index = 0; index < _size; ++index) {
				hash ^= _data[_size];
				hash *= 0x00000100000001B3;
			}
			return hash;
		}
	}  // namespace Detail
}  // namespace Orion::Engine
