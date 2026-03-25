#pragma once

#include "Core/Standard/TypeTraits.h"

namespace Orion::Engine
{
	template <typename T, typename U>
	concept SameAs = IsSame<T, U>;
}
