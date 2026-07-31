#pragma once

#include "OrionEngine.h"

#include "Core/Standard/TypeTraits.h"

namespace Orion::Engine
{
	/// @brief Concept that is satisfied if and only if types T and U denote the same type.
	template <typename T, typename U>
	concept SameAs = IsSame<T, U> && IsSame<U, T>;

	template <typename T>
	concept IsInteger = SameAs<T, UInt8> ||   //
	                    SameAs<T, UInt16> ||  //
	                    SameAs<T, UInt32> ||  //
	                    SameAs<T, UInt64> ||  //
	                    SameAs<T, Int8> ||    //
	                    SameAs<T, Int16> ||   //
	                    SameAs<T, Int32> ||   //
	                    SameAs<T, Int64>      //
		;
}  // namespace Orion::Engine
