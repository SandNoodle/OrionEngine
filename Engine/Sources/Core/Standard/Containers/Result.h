#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Containers/String.h"
#include "Core/Standard/TypeTraits.h"
#include "Core/Standard/Utility/MoveAndForward.h"

namespace Orion::Engine
{
	/// @brief Represents generic Error type.
	class Error final
	{
		private:
		String _message{};

		public:
		constexpr explicit Error(const String& message) noexcept : _message(message) {}
		constexpr explicit Error(String&& message) noexcept : _message(Move(message)) {}

		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 HasMessage() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr StringView Message() const noexcept;
	};

	/// @brief Represents type-safe container over two \tparam T and \tparam E types.
	/// In reality, it's just a thin wrapper around C-styled union, with additional member for differentiation which
	/// value type is present.
	/// @tparam T Expected type to be stored.
	/// @tparam E Error type to be stored, if expected type is not present.
	template <typename T, typename E = Error>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	class Result final
	{
		public:
		using ValueType = T;
		using ErrorType = E;

		private:
		union
		{
			RemoveConst<ValueType> _value;
			RemoveConst<ErrorType> _error;
		};
		Bool8 _has_value;

		public:
		constexpr Result(const ValueType&) noexcept;
		constexpr Result(ValueType&&) noexcept;
		constexpr Result(const ErrorType&) noexcept;
		constexpr Result(ErrorType&&) noexcept;
		constexpr Result(const Result&) noexcept = default;
		constexpr Result(Result&&) noexcept      = default;
		constexpr ~Result() noexcept;

		constexpr Result& operator=(ValueType&&) noexcept;
		constexpr Result& operator=(const ValueType&) noexcept;
		constexpr Result& operator=(ErrorType&&) noexcept;
		constexpr Result& operator=(const ErrorType&) noexcept;
		constexpr Result& operator=(const Result&) noexcept = default;
		constexpr Result& operator=(Result&&) noexcept      = default;
		ORION_FORCE_INLINE constexpr ValueType& operator*() & noexcept;
		ORION_FORCE_INLINE constexpr const ValueType& operator*() const& noexcept;
		ORION_FORCE_INLINE constexpr ValueType&& operator*() && noexcept;
		ORION_FORCE_INLINE constexpr const ValueType&& operator*() const&& noexcept;
		ORION_FORCE_INLINE constexpr ValueType* operator->() noexcept;
		ORION_FORCE_INLINE constexpr const ValueType* operator->() const noexcept;

		/// @brief Checks if Result contains a value type at this given moment.
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 IsValue() const noexcept;

		/// @brief Checks if Result contains an error type at this given moment.
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 IsError() const noexcept;

		/// @brief Returns value contained by this Result.
		/// @warning Assumes that IsValue() has returned true - otherwise the behavior is not defined.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE constexpr const ValueType& Value() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ValueType& Value() noexcept;
		/// @}

		/// @brief Returns value contained by this Result if it is present, or \p value otherwise.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE constexpr ValueType& ValueOr(const ValueType& value) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ValueType& ValueOr(ValueType&& value) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr const ValueType& ValueOr(const ValueType& value) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr const ValueType& ValueOr(ValueType&& value) const noexcept;
		/// @}

		/// @brief Returns value contained by this Result if it is present, or default value for type otherwise.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE constexpr ValueType& ValueOrDefault() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr const ValueType& ValueOrDefault() const noexcept;
		/// @}

		/// @brief Returns error value contained by this Result.
		/// @warning Assumes that IsError() has returned true - otherwise the behavior is not defined.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE constexpr const ErrorType& Error() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ErrorType& Error() noexcept;
		/// @}
	};

	// -- Implementation.
	ORION_FORCE_INLINE constexpr auto Error::HasMessage() const noexcept -> Bool8
	{
		return _message.IsEmpty();
	}
	ORION_FORCE_INLINE constexpr auto Error::Message() const noexcept -> StringView
	{
		return StringView(_message.Data(), _message.Size());
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	constexpr Result<T, E>::Result(const ValueType& value) noexcept : _value(value), _has_value(true)
	{
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	constexpr Result<T, E>::Result(ValueType&& value) noexcept : _value(Move(value)), _has_value(true)
	{
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	constexpr Result<T, E>::Result(const ErrorType& error) noexcept : _error(error), _has_value(false)
	{
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	constexpr Result<T, E>::Result(ErrorType&& error) noexcept : _error(Move(error)), _has_value(false)
	{
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	constexpr Result<T, E>::~Result() noexcept
	{
		_has_value ? _value.~ValueType() : _error.~ErrorType();
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	constexpr auto Result<T, E>::operator=(ValueType&& value) noexcept -> Result&
	{
		ORION_IGNORE_PARAM(value);
		ORION_NOT_IMPLEMENTED();
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	constexpr auto Result<T, E>::operator=(const ValueType& value) noexcept -> Result&
	{
		ORION_IGNORE_PARAM(value);
		ORION_NOT_IMPLEMENTED();
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	constexpr auto Result<T, E>::operator=(ErrorType&& error) noexcept -> Result&
	{
		ORION_IGNORE_PARAM(error);
		ORION_NOT_IMPLEMENTED();
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	constexpr auto Result<T, E>::operator=(const ErrorType& error) noexcept -> Result&
	{
		ORION_IGNORE_PARAM(error);
		ORION_NOT_IMPLEMENTED();
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	ORION_FORCE_INLINE constexpr auto Result<T, E>::operator*() & noexcept -> ValueType&
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	ORION_FORCE_INLINE constexpr auto Result<T, E>::operator*() const& noexcept -> const ValueType&
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	ORION_FORCE_INLINE constexpr auto Result<T, E>::operator*() && noexcept -> ValueType&&
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	ORION_FORCE_INLINE constexpr auto Result<T, E>::operator*() const&& noexcept -> const ValueType&&
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	ORION_FORCE_INLINE constexpr auto Result<T, E>::operator->() noexcept -> ValueType*
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return &_value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	ORION_FORCE_INLINE constexpr auto Result<T, E>::operator->() const noexcept -> const ValueType*
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return &_value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	constexpr auto Result<T, E>::IsValue() const noexcept -> Bool8
	{
		return _has_value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	constexpr auto Result<T, E>::IsError() const noexcept -> Bool8
	{
		return !_has_value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	[[nodiscard]] ORION_FORCE_INLINE constexpr auto Result<T, E>::Value() const noexcept -> const ValueType&
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	[[nodiscard]] ORION_FORCE_INLINE constexpr auto Result<T, E>::Value() noexcept -> ValueType&
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	[[nodiscard]] ORION_FORCE_INLINE constexpr auto Result<T, E>::ValueOr(const ValueType& value) noexcept -> ValueType&
	{
		return _has_value ? _value : value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	[[nodiscard]] ORION_FORCE_INLINE constexpr auto Result<T, E>::ValueOr(ValueType&& value) noexcept -> ValueType&
	{
		return _has_value ? _value : value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	[[nodiscard]] ORION_FORCE_INLINE constexpr auto Result<T, E>::ValueOr(const ValueType& value) const noexcept
		-> const ValueType&
	{
		return _has_value ? _value : value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	[[nodiscard]] ORION_FORCE_INLINE constexpr auto Result<T, E>::ValueOr(ValueType&& value) const noexcept
		-> const ValueType&
	{
		return _has_value ? _value : value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	[[nodiscard]] ORION_FORCE_INLINE constexpr auto Result<T, E>::ValueOrDefault() noexcept -> ValueType&
	{
		return _has_value ? _value : ValueType();
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	[[nodiscard]] ORION_FORCE_INLINE constexpr auto Result<T, E>::ValueOrDefault() const noexcept -> const ValueType&
	{
		return _has_value ? _value : ValueType();
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	[[nodiscard]] ORION_FORCE_INLINE constexpr auto Result<T, E>::Error() const noexcept -> const ErrorType&
	{
		ORION_ASSERT_DEBUG_SLOW(!_has_value);
		return _error;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E> && !IsSame<T, E>)
	[[nodiscard]] ORION_FORCE_INLINE constexpr auto Result<T, E>::Error() noexcept -> ErrorType&
	{
		ORION_ASSERT_DEBUG_SLOW(!_has_value);
		return _error;
	}
}  // namespace Orion::Engine
