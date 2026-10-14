#pragma once

namespace Orion::Engine
{
	namespace Detail
	{
		// clang-format off
		template <typename T> struct RemoveReference { using Type = T; };
		template <typename T> struct RemoveReference<T&> { using Type = T; };
		template <typename T> struct RemoveReference<const T&> { using Type = T; };
		// clang-format on

		// clang-format off
		template <typename T> struct RemoveConst { using Type = T; };
		template <typename T> struct RemoveConst<const T> { using Type = T; };
		// clang-format on

		// clang-format off
		template <typename T> struct RemoveVolatile { using Type = T; };
		template <typename T> struct RemoveVolatile<volatile T> { using Type = T; };
		// clang-format on

		// clang-format off
		template <typename T> struct RemoveConstVolatile { using Type = T; };
		template <typename T> struct RemoveConstVolatile<const T> { using Type = T; };
		template <typename T> struct RemoveConstVolatile<volatile T> { using Type = T; };
		template <typename T> struct RemoveConstVolatile<const volatile T> { using Type = T; };
		// clang-format on

		// clang-format off
		template <typename T> struct RemovePointer { using Type = T; };
		template <typename T> struct RemovePointer<T*> { using Type = T; };
		template <typename T> struct RemovePointer<const T*> { using Type = T; };
		template <typename T> struct RemovePointer<volatile T*> { using Type = T; };
		template <typename T> struct RemovePointer<const volatile T*> { using Type = T; };
		// clang-format on

		// clang-format off
		template <typename T> struct AddConst { using Type = const T; };
		template <typename T> struct AddVolatile { using Type = volatile T; };
		template <typename T> struct AddConstVolatile { using Type = const volatile T; };
		// clang-format on

		// NOLINTBEGIN(readability-identifier-naming)
		// clang-format off
		template <class T> inline constexpr bool IsLValueReference     = false;
		template <class T> inline constexpr bool IsLValueReference<T&> = true;
		// clang-format on

		// clang-format off
		template <class T> inline constexpr bool IsRValueReference      = false;
		template <class T> inline constexpr bool IsRValueReference<T&&> = true;
		// clang-format on

		// clang-format off
		template <typename T> inline constexpr bool IsVolatile                    = false;
		template <typename T> inline constexpr bool IsVolatile<volatile T>        = true;
		template <typename T> inline constexpr bool IsVolatile<const volatile T>  = true;
		// clang-format on

		// clang-format off
		template <typename T> inline constexpr bool IsConst                    = false;
		template <typename T> inline constexpr bool IsConst<const T>           = true;
		template <typename T> inline constexpr bool IsConst<const volatile T>  = true;
		// clang-format on

		// clang-format off
		template <typename T> inline constexpr bool IsPointer                    = false;
		template <typename T> inline constexpr bool IsPointer<T*>                = true;
		template <typename T> inline constexpr bool IsPointer<const T*>          = true;
		template <typename T> inline constexpr bool IsPointer<volatile T*>       = true;
		template <typename T> inline constexpr bool IsPointer<const volatile T*> = true;
		// clang-format on
		// NOLINTEND(readability-identifier-naming)
	}  // namespace Detail

	/// @brief Attempts to remove a reference qualifier from a given type (if present).
	template <typename T>
	using RemoveReference = Detail::RemoveReference<T>::Type;

	/// @brief Attempts to remove a const qualifier from a given type (if present).
	template <typename T>
	using RemoveConst = Detail::RemoveConst<T>::Type;

	/// @brief Attempts to remove a volatile qualifier from a given type (if present).
	template <typename T>
	using RemoveVolatile = Detail::RemoveVolatile<T>::Type;

	/// @brief Attempts to remove both const and volatile qualifiers from a type (if present).
	template <typename T>
	using RemoveConstVolatile = Detail::RemoveConstVolatile<T>::Type;

	/// @brief Attempts to remove const, volatile and reference qualifiers from a type (if present).
	template <typename T>
	using RemoveConstVolatileReference = RemoveConstVolatile<RemoveReference<T>>;

	/// @brief Attempts to add const qualifier to a type (if possible).
	template <typename T>
	using AddConst = Detail::AddConst<T>::Type;

	/// @brief Attempts to add volatile qualifier to a type (if possible).
	template <typename T>
	using AddVolatile = Detail::AddVolatile<T>::Type;

	/// @brief Attempts to add both const and volatile qualifiers to a type (if possible).
	template <typename T>
	using AddConstVolatile = Detail::AddConstVolatile<T>::Type;

	/// @brief Attempts to remove a pointer from a type (if present).
	template <typename T>
	using RemovePointer = Detail::RemovePointer<T>::Type;

	// NOLINTBEGIN(readability-identifier-naming)
	/// @brief Checks if a given type is an L-Value reference, evaluating to `true` if it is.
	template <class T>
	inline constexpr bool IsLValueReference = Detail::IsLValueReference<T>;

	/// @brief Checks if a given type is an R-Value reference, evaluating to `true` if it is.
	template <class T>
	inline constexpr bool IsRValueReference = Detail::IsRValueReference<T>;

	/// @brief Checks if a given type is a volatile or const volatile type, evaluating to `true` if it is.
	template <class T>
	inline constexpr bool IsVolatile = Detail::IsVolatile<T>;

	/// @brief Checks if a given type is a const or const volatile type, evaluating to `true` if it is.
	template <class T>
	inline constexpr bool IsConst = Detail::IsConst<T>;

	/// @brief Checks if a given type is a pointer type, evaluating to `true` if it is.
	template <typename T>
	inline constexpr bool IsPointer = Detail::IsPointer<T>;
	// NOLINTEND(readability-identifier-naming)
}  // namespace Orion::Engine
