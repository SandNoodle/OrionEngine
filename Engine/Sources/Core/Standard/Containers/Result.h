#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Containers/String.h"
#include "Core/Standard/TypeTraits.h"
#include "Core/Standard/Utility/MoveAndForward.h"

namespace Orion::Engine
{
	class Error final
	{
		private:
		String _message{};

		public:
		constexpr Error() noexcept {}
		constexpr explicit Error(const String& message) noexcept : _message(message) {}
		constexpr explicit Error(String&& message) noexcept : _message(Move(message)) {}

		[[nodiscard]] ORION_FORCE_INLINE constexpr StringView Message() const noexcept
		{
			return StringView(_message.Data(), _message.Size());
		}
	};

	template <typename T, typename E = Error>
		requires(!IsLValueReference<T> && !IsLValueReference<E>)
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

		constexpr Result& operator=(ValueType&& value) noexcept;
		constexpr Result& operator=(const ValueType& value) noexcept;
		constexpr Result& operator=(ErrorType&& value) noexcept;
		constexpr Result& operator=(const ErrorType& value) noexcept;
		constexpr Result& operator=(const Result&) noexcept = default;
		constexpr Result& operator=(Result&&) noexcept      = default;

		/// @brief TODO
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 IsValue() const noexcept;

		/// @brief TODO
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 IsError() const noexcept;

		/// @brief TODO
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE constexpr const ValueType& Value() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ValueType& Value() noexcept;
		/// @}

		/// @brief TODO
		/// @{
		[[nodiscard]] constexpr ValueType& ValueOr(const ValueType&) noexcept;
		[[nodiscard]] constexpr ValueType& ValueOr(ValueType&&) noexcept;
		[[nodiscard]] constexpr const ValueType& ValueOr(const ValueType&) const noexcept;
		[[nodiscard]] constexpr const ValueType& ValueOr(ValueType&&) const noexcept;
		/// @}

		/// @brief TODO
		/// @{
		[[nodiscard]] constexpr ValueType& ValueOrDefault() noexcept;
		[[nodiscard]] constexpr const ValueType& ValueOrDefault() const noexcept;
		/// @}

		/// @brief TODO
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE constexpr const ErrorType& Error() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ErrorType& Error() noexcept;
		/// @}
	};

	// -- Implementation.

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E>)
	constexpr Result<T, E>::Result(const ValueType& value) noexcept : _value(value), _has_value(true)
	{
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E>)
	constexpr Result<T, E>::Result(ValueType&& value) noexcept : _value(Move(value)), _has_value(true)
	{
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E>)
	constexpr Result<T, E>::Result(const ErrorType& error) noexcept : _error(error), _has_value(false)
	{
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E>)
	constexpr Result<T, E>::Result(ErrorType&& error) noexcept : _error(Move(error)), _has_value(false)
	{
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E>)
	constexpr Result<T, E>::~Result() noexcept
	{
		_has_value ? _value.~ValueType() : _error.~ErrorType();
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E>)
	constexpr auto Result<T, E>::IsValue() const noexcept -> Bool8
	{
		return _has_value;
	}

	template <typename T, typename E>
		requires(!IsLValueReference<T> && !IsLValueReference<E>)
	constexpr auto Result<T, E>::IsError() const noexcept -> Bool8
	{
		return !_has_value;
	}
}  // namespace Orion::Engine
