#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Algorithms/Hash.h"

namespace Orion::Engine
{
	namespace Detail
	{
		template <typename CharT>
		class StringViewBase
		{
			public:
			using ThisType           = StringViewBase<CharT>;
			using ValueType          = CharT;
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
			constexpr explicit StringViewBase(const char* data, SizeType size) noexcept;

			/// @brief Constructs a new StringView from a C-style string literal.
			[[nodiscard]] ORION_FORCE_INLINE static constexpr StringViewBase FromStringLiteral(const char*) noexcept;

			[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType operator[](SizeType index) noexcept;
			[[nodiscard]] ORION_FORCE_INLINE constexpr ConstReferenceType operator[](SizeType index) const noexcept;

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

	using StringView = Detail::StringViewBase<Char8>;

	// -- Deduction guides.
	// TODO(SandNoodle): Deduction guides.

	// -- Hash
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
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::Front() noexcept -> ReferenceType
		{
			return *_data;
		}

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::Front() const noexcept -> ConstReferenceType
		{
			return *_data;
		}

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::Back() noexcept -> ReferenceType
		{
			return *_data[_size - 1];
		}

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::Back() const noexcept -> ConstReferenceType
		{
			return *_data[_size - 1];
		}

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::Data() noexcept -> PointerType
		{
			return _data;
		}

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::Data() const noexcept -> ConstPointerType
		{
			return _data;
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

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::IsEmpty() const noexcept -> Bool8
		{
			return _size == 0;
		}

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::Size() const noexcept -> SizeType
		{
			return _size;
		}

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::ByteSize() const noexcept -> SizeType
		{
			return _size * sizeof(ValueType);
		}

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::operator[](SizeType index) noexcept -> ReferenceType
		{
			ORION_ASSERT_DEBUG(index < _size);
			return _data[index];
		}

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::operator[](SizeType index) const noexcept
			-> ConstReferenceType
		{
			ORION_ASSERT_DEBUG(index < _size);
			return _data[index];
		}

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::begin() noexcept -> PointerType
		{
			return &_data[0];
		}

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::begin() const noexcept -> ConstPointerType
		{
			return &_data[0];
		}

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::end() noexcept -> PointerType
		{
			return &_data[_size - 1];
		}

		template <typename CharT>
		ORION_FORCE_INLINE constexpr auto StringViewBase<CharT>::end() const noexcept -> ConstPointerType
		{
			return &_data[_size - 1];
		}
	}  // namespace Detail
}  // namespace Orion::Engine
