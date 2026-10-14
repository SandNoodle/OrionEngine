#pragma once

namespace Orion::Engine
{
	/// @brief TypeConstant is a thin wrapper for a static constant of a given type.
	template <class T, T Value>
	struct TypeConstant
	{
		public:
		using ValueType = T;
		using Type      = TypeConstant;

		static constexpr ValueType k_value = Value;

		public:
		constexpr operator ValueType() const;
		constexpr ValueType operator()() const;
	};

	using TrueType  = TypeConstant<bool, true>;
	using FalseType = TypeConstant<bool, false>;

	// -- Implementation.
	template <class T, T Value>
	constexpr TypeConstant<T, Value>::operator ValueType() const
	{
		return k_value;
	}

	template <class T, T Value>
	constexpr auto TypeConstant<T, Value>::operator()() const -> ValueType
	{
		return k_value;
	}
}  // namespace Orion::Engine
