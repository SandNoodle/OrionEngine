#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Concepts.h"

namespace Orion::Engine::Algorithm
{
	/**
	 * @brief TODO
	 * @tparam T Type to be hashed.
	 */
	template <typename T>
	struct Hash
	{
		public:
		using ValueType = T;
		using SizeType  = USize;

		public:
		SizeType operator()(const ValueType& v) const
		{
			ORION_IGNORE_PARAM(v);
			ORION_NOT_IMPLEMENTED("Hash<...> is not implemented for type.");
		}
	};

#define ORION_PRIMITIVE_TYPE_HASH_LIST \
	ORION_PRIMITIVE_HASH(UInt8)        \
	ORION_PRIMITIVE_HASH(UInt16)       \
	ORION_PRIMITIVE_HASH(UInt32)       \
	ORION_PRIMITIVE_HASH(UInt64)       \
	ORION_PRIMITIVE_HASH(Int8)         \
	ORION_PRIMITIVE_HASH(Int16)        \
	ORION_PRIMITIVE_HASH(Int32)        \
	ORION_PRIMITIVE_HASH(Int64)        \
	ORION_PRIMITIVE_HASH(Float32)      \
	ORION_PRIMITIVE_HASH(Float64)

#define ORION_PRIMITIVE_HASH(type)                    \
	template <>                                       \
	struct Hash<type>                                 \
	{                                                 \
		public:                                       \
		using ValueType = type;                       \
		using SizeType  = USize;                      \
                                                      \
		public:                                       \
		SizeType operator()(const ValueType& v) const \
		{                                             \
			return static_cast<SizeType>(v);          \
		}                                             \
	};
	ORION_PRIMITIVE_TYPE_HASH_LIST
#undef ORION_PRIMITIVE_HASH
#undef ORION_PRIMITIVE_TYPE_HASH_LIST
}  // namespace Orion::Engine::Algorithm
