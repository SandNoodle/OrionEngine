#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Memory/Lifetime.h"
#include "Core/Standard/TypeTraits.h"
#include "Core/Standard/Utility/MoveAndForward.h"

namespace Orion::Engine
{
	namespace Detail
	{
		template <typename T>
		concept OptionalKind = !IsLValueReference<T> && !IsSame<T, void>;

		/// @brief Represents a no-value type.
		struct OptionalNull
		{
			constexpr explicit OptionalNull(UInt8) {}
		};
	}  // namespace Detail

	static constexpr Detail::OptionalNull k_null_option = Detail::OptionalNull{ 0 };

	/// @brief Represents a type-safe nullable value.
	/// In reality, it's just a thin wrapper around C-styled union, with additional member for differentiation which
	/// value type is present.
	/// @tparam T Type to be stored.
	template <typename T>
		requires(Detail::OptionalKind<T>)
	class Optional final
	{
		public:
		using ValueType          = T;
		using ReferenceType      = ValueType&;
		using ConstReferenceType = const ValueType&;
		using PointerType        = ValueType*;
		using ConstPointerType   = const ValueType*;

		private:
		union
		{
			Detail::OptionalNull _null_value;
			ValueType _value;
		};
		Bool8 _has_value;

		public:
		constexpr Optional() noexcept;
		constexpr Optional(const ValueType& value) noexcept;
		constexpr Optional(ValueType&& value) noexcept;
		constexpr Optional(Detail::OptionalNull);
		constexpr Optional(const Optional&) noexcept;
		constexpr Optional(Optional&&) noexcept;
		constexpr ~Optional();

		constexpr Optional& operator=(ValueType&&) noexcept;
		constexpr Optional& operator=(const ValueType&) noexcept;
		constexpr Optional& operator=(const Optional&) noexcept;
		constexpr Optional& operator=(Optional&&) noexcept;
		ORION_FORCE_INLINE constexpr ReferenceType operator*() & noexcept;
		ORION_FORCE_INLINE constexpr ConstReferenceType operator*() const& noexcept;
		ORION_FORCE_INLINE constexpr ValueType&& operator*() && noexcept;
		ORION_FORCE_INLINE constexpr const ValueType&& operator*() const&& noexcept;
		ORION_FORCE_INLINE constexpr PointerType operator->() noexcept;
		ORION_FORCE_INLINE constexpr ConstPointerType operator->() const noexcept;

		/// @brief Checks if Optional contains a value type a this given moment.
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 IsValue() const noexcept;

		/// @brief Checks if Optional contains a null value type a this given moment.
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 IsNull() const noexcept;

		/// @brief Returns value contained by this Optional.
		/// @warning Assumes that IsValue() has returned true - otherwise the behavior is not defined.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE constexpr const ValueType& Value() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ValueType& Value() noexcept;
		/// @}

		/// @brief Returns value contained by this Optional if it is present, or \p value otherwise.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE constexpr ValueType ValueOr(const ValueType& value) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ValueType ValueOr(ValueType&& value) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ValueType ValueOr(const ValueType& value) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ValueType ValueOr(ValueType&& value) const noexcept;
		/// @}

		/// @brief Returns value contained by this Optional if it is present, or default value for type otherwise.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE constexpr ValueType ValueOrDefault() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ValueType ValueOrDefault() const noexcept;
		/// @}
	};

	// -- Implementation.
	template <typename T>
		requires(Detail::OptionalKind<T>)
	constexpr Optional<T>::Optional() noexcept : _value(ValueType()), _has_value(true)
	{
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	constexpr Optional<T>::Optional(const ValueType& value) noexcept : _value(value), _has_value(true)
	{
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	constexpr Optional<T>::Optional(ValueType&& value) noexcept : _value(Move(value)), _has_value(true)
	{
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	constexpr Optional<T>::Optional(Detail::OptionalNull) : _null_value(k_null_option), _has_value(false)
	{
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	constexpr Optional<T>::Optional(const Optional& other) noexcept : _has_value(other._has_value)
	{
		if (_has_value) {
			Memory::ConstructItem(&_value, other._value);
		} else {
			Memory::ConstructItem(&_null_value, k_null_option);
		}
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	constexpr Optional<T>::Optional(Optional&& other) noexcept : _has_value(other._has_value)
	{
		if (_has_value) {
			Memory::ConstructItem(&_value, Move(other._value));
		} else {
			Memory::ConstructItem(&_null_value, k_null_option);
		}
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	constexpr Optional<T>::~Optional()
	{
		if (_has_value) {
			Memory::DestructItems(&_value, 1);
		}
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	constexpr auto Optional<T>::operator=(ValueType&& value) noexcept -> Optional&
	{
		if (_has_value) {
			Memory::DestructItems(&_value, 1);
		}
		Memory::ConstructItem(&_value, Move(value));
		_has_value = true;
		return *this;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	constexpr auto Optional<T>::operator=(const ValueType& value) noexcept -> Optional&
	{
		if (_has_value) {
			Memory::DestructItems(&_value, 1);
		}
		Memory::ConstructItem(&_value, value);
		_has_value = true;
		return *this;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	constexpr auto Optional<T>::operator=(const Optional& other) noexcept -> Optional&
	{
		if (this != &other) {
			if (_has_value) {
				Memory::DestructItems(&_value, 1);
			}
			_has_value = other._has_value;
			if (_has_value) {
				Memory::ConstructItem(&_value, other._value);
			} else {
				Memory::ConstructItem(&_null_value, k_null_option);
			}
		}
		return *this;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	constexpr auto Optional<T>::operator=(Optional&& other) noexcept -> Optional&
	{
		if (this != &other) {
			if (_has_value) {
				Memory::DestructItems(&_value, 1);
			}
			_has_value = other._has_value;
			if (_has_value) {
				Memory::ConstructItem(&_value, Move(other._value));
			} else {
				Memory::ConstructItem(&_null_value, k_null_option);
			}
		}
		return *this;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::operator*() & noexcept -> ReferenceType
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::operator*() const& noexcept -> ConstReferenceType
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::operator*() && noexcept -> ValueType&&
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::operator*() const&& noexcept -> const ValueType&&
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::operator->() noexcept -> PointerType
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return &_value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::operator->() const noexcept -> ConstPointerType
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return &_value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::IsValue() const noexcept -> Bool8
	{
		return _has_value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::IsNull() const noexcept -> Bool8
	{
		return !_has_value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::Value() const noexcept -> const ValueType&
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::Value() noexcept -> ValueType&
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::ValueOr(const ValueType& value) noexcept -> ValueType
	{
		return _has_value ? _value : value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::ValueOr(ValueType&& value) noexcept -> ValueType
	{
		return _has_value ? _value : value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::ValueOr(const ValueType& value) const noexcept -> ValueType
	{
		return _has_value ? _value : value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::ValueOr(ValueType&& value) const noexcept -> ValueType
	{
		return _has_value ? _value : value;
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::ValueOrDefault() noexcept -> ValueType
	{
		return _has_value ? _value : ValueType();
	}

	template <typename T>
		requires(Detail::OptionalKind<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::ValueOrDefault() const noexcept -> ValueType
	{
		return _has_value ? _value : ValueType();
	}
}  // namespace Orion::Engine
