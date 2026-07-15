#pragma once

#include "OrionEngine.h"

namespace Orion::Engine
{
	template <typename T>
	struct Hash
	{
		public:
		using ValueType = T;
		using SizeType  = USize;

		public:
		SizeType operator()(const ValueType& v)
		{
			ORION_IGNORE_PARAM(v);
			ORION_NOT_IMPLEMENTED("Hash<...> is not implemented for type.");
		}
	};
}  // namespace Orion::Engine
