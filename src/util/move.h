#pragma once

#include "platform/compiler_macros.h"
#include "util/remove_reference.h"

namespace orion::util
{
	/**
	 * @brief Used to indicate that the object can be 'moved' from,
	 * i.e. allowing the efficient transfer of resources from x
	 * to another object.
	 *
	 * @tparam T Type of the variable.
	 * @param x Value of the variable.
	 *
	 * @return object to be moved.
	 */
	template <typename T>
	ORION_CONSTEXPR typename remove_reference<T>::type&& move(T&& x) noexcept
	{
		return static_cast<typename remove_reference<T>::type&&>(x);
	}
}  // namespace orion::util

