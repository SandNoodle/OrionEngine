#pragma once

#include "platform/compiler_macros.h"
#include "platform/types.h"

namespace orion::util
{
	/**
	 * @brief Wrapper for static constants of specified type.
	 * @tparam T Base type of a constant.
	 * @tparam v Value of a constant.
	 */
	template <class T, T v>
	struct integral_constant
	{
		typedef T                       value_type;
		typedef integral_constant<T, v> type;

		static ORION_CONSTEXPR T value = v;

		ORION_CONSTEXPR operator value_type() const   { return value; }
		ORION_CONSTEXPR value_type operator()() const { return value; }
	};

	template <b8 B>
	using bool_constant = integral_constant<b8, B>;

	typedef bool_constant<true>  true_type;
	typedef bool_constant<false> false_type;

	/**
	 * @brief Provides member constant value equal to T == U.
	 * @tparam T LHS Type.
	 * @tparam U RHS Type.
	 */
	template <class T, class U>
	struct is_same : false_type {};

	template <class T>
	struct is_same<T, T> : true_type {};

	template <class T, class U>
	ORION_INLINE ORION_CONSTEXPR auto is_same_v = is_same<T, U>::value;

}
