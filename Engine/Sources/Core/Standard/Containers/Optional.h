#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/TypeTraits.h"

namespace Orion::Engine
{
	namespace Detail
	{
		/// @brief TODO
		struct OptionalNull
		{
			constexpr explicit OptionalNull(UInt8) {}
		};
	}  // namespace Detail

	static constexpr Detail::OptionalNull k_null_option = Detail::OptionalNull{ 0 };

	/// @brief TODO
	template <typename T>
		requires(!IsLValueReference<T>)
	class Optional final
	{
		public:
		using ValueType          = T;
		using ReferenceType      = ValueType&;
		using ConstReferenceType = const ValueType&;
		using PointerType        = ValueType&;
		using ConstPointerType   = const ValueType&;

		private:
		union
		{
			RemoveConst<Detail::OptionalNull> _null_value;
			RemoveConst<ValueType> _value;
		};
		Bool8 _has_value;

		public:
		constexpr Optional(const ValueType& value) noexcept : _value(value), _has_value(true) {}
		constexpr Optional(ValueType&& value) noexcept : _value(Move(value)), _has_value(true) {}
		constexpr Optional(Detail::OptionalNull) : _null_value(k_null_option), _has_value(false) {}
		constexpr ~Optional();

		ORION_FORCE_INLINE constexpr ReferenceType operator*() noexcept;
		ORION_FORCE_INLINE constexpr ConstReferenceType operator*() const noexcept;

		/// @brief TODO
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 IsValue() const noexcept;
	};

	// -- Implementation.
	template <typename T>
		requires(!IsLValueReference<T>)
	constexpr Optional<T>::~Optional()
	{
		if constexpr (!IsTriviallyDestructible<ValueType>) {
			if (_has_value) {
				ValueType::~ValueType();
			}
		}
	}

	template <typename T>
		requires(!IsLValueReference<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::operator*() noexcept -> ReferenceType
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T>
		requires(!IsLValueReference<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::operator*() const noexcept -> ConstReferenceType
	{
		ORION_ASSERT_DEBUG_SLOW(_has_value);
		return _value;
	}

	template <typename T>
		requires(!IsLValueReference<T>)
	ORION_FORCE_INLINE constexpr auto Optional<T>::IsValue() const noexcept -> Bool8
	{
		return _has_value;
	}
}  // namespace Orion::Engine
