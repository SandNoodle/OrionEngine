#pragma once

#include "platform/compiler_macros.h"
#include "util/remove_reference.h"
#include "util/type_traits.h"

namespace orion::util
{
	/**
	 * @brief Forwards the value to another function just as it was passed to the caller.
	 * @tparam T Type of the variable.
	 * @param x Value of the variable.
	 * @return Perfect forwarding reference.
	 */
	template <class T>
	ORION_CONSTEXPR auto forward(typename util::remove_reference<T>::type& x) noexcept -> T&&
	{
		return static_cast<T&&>(x);
	}

	/**
	 * @brief Forwards the value to another function just as it was passed to the caller.
	 * @tparam T Type of the variable.
	 * @param x Value of the variable.
	 * @return Perfect forwarding reference.
	 */
	template <class T>
	ORION_CONSTEXPR auto forward(typename util::remove_reference<T>::type&& x) noexcept -> T&&
	{
		// TODO: Does this work?
		static_assert(!util::is_same<T, T&>::value, "util::forward: cannot forward rvalue as an lvalue.");
		return static_cast<T&&>(x);
	}
}
