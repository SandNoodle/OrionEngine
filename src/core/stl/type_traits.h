#pragma once

#include "platform/types.h"
#include "platform/compiler_macros.h"

namespace orion
{
	//
	// integral_constant
	//
	// @TODO descripiton.
	//
	template <class T, T v>
	struct integral_constant
	{
		static ORION_CONSTEXPR T value = v;
		typedef T value_type;
		typedef integral_constant<T, v> type;
		ORION_CONSTEXPR operator value_type() const noexcept   { return value; }
		ORION_CONSTEXPR value_type operator()() const noexcept { return value; }
	};

	//
	// true_type / false_type 
	//
	// @TODO descripiton.
	//
	typedef integral_constant<b8, true>  true_type;
	typedef integral_constant<b8, false> false_type;

	//
	// b8 constant
	//
	// @TODO descripiton.
	//
	template<b8 B>
	using b8_constant = integral_constant<b8, B>;

	//
	// is_same
	//
	// @TODO descripiton
	//
	template <class T, class U>
	struct is_same : false_type {};

	template <class T>
	struct is_same<T, T> : true_type {};
}
