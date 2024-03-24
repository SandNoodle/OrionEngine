#pragma once

#include "platform/compiler_macros.h"
#include "core/stl/remove_reference.h"

namespace orion
{
	//
	// move
	//
	// @TODO description
	//
	template <typename T>
	ORION_CONSTEXPR typename remove_reference<T>::type&& move(T&& x) noexcept
	{
		return static_cast<typename remove_reference<T>::type&&>(x);
	}

}
