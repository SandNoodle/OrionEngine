#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"

namespace Orion::Engine::Math
{
	namespace Detail
	{
		/// @brief TODO
		/// @tparma T TODO
		template <typename T>
		struct Vector2Base
		{
			public:
			using ThisType           = Vector2Base;
			using ValueType          = T;
			using SizeType           = USize;
			using ReferenceType      = ValueType&;
			using ConstReferenceType = const ValueType&;

			public:
			union
			{
				ValueType _data[2];
				struct
				{
					ValueType x, y;
				};
			};

			public:
			[[nodiscard]] constexpr ReferenceType operator[](SizeType index) noexcept;
			[[nodiscard]] constexpr ValueType operator[](SizeType index) const noexcept;
		};

		/// @brief TODO
		/// @tparma T TODO
		template <typename T>
		struct Vector3Base
		{
			public:
			using ThisType           = Vector3Base;
			using ValueType          = T;
			using SizeType           = USize;
			using ReferenceType      = ValueType&;
			using ConstReferenceType = const ValueType&;

			public:
			union
			{
				ValueType _data[3];
				struct
				{
					ValueType x, y, z;
				};
			};

			public:
			[[nodiscard]] constexpr ReferenceType operator[](SizeType index) noexcept;
			[[nodiscard]] constexpr ValueType operator[](SizeType index) const noexcept;
		};

		/// @brief TODO
		/// @tparma T TODO
		template <typename T>
		struct Vector4Base
		{
			public:
			using ThisType           = Vector4Base;
			using ValueType          = T;
			using SizeType           = USize;
			using ReferenceType      = ValueType&;
			using ConstReferenceType = const ValueType&;

			public:
			union
			{
				ValueType _data[4];
				struct
				{
					ValueType x, y, z, w;
				};
			};

			public:
			[[nodiscard]] constexpr ReferenceType operator[](SizeType index) noexcept;
			[[nodiscard]] constexpr ValueType operator[](SizeType index) const noexcept;
		};
	}  // namespace Detail

	/// @brief TODO
#define ORION_MATH_VECTOR_TYPE_LIST \
	ORION_MATH_VECTOR_TYPE(Bool8)   \
	ORION_MATH_VECTOR_TYPE(Bool16)  \
	ORION_MATH_VECTOR_TYPE(Bool32)  \
	ORION_MATH_VECTOR_TYPE(Bool64)  \
	ORION_MATH_VECTOR_TYPE(UInt8)   \
	ORION_MATH_VECTOR_TYPE(UInt16)  \
	ORION_MATH_VECTOR_TYPE(UInt64)  \
	ORION_MATH_VECTOR_TYPE(UInt32)  \
	ORION_MATH_VECTOR_TYPE(UInt64)  \
	ORION_MATH_VECTOR_TYPE(Int8)    \
	ORION_MATH_VECTOR_TYPE(Int16)   \
	ORION_MATH_VECTOR_TYPE(Int64)   \
	ORION_MATH_VECTOR_TYPE(Int32)   \
	ORION_MATH_VECTOR_TYPE(Int64)   \
	ORION_MATH_VECTOR_TYPE(Float32) \
	ORION_MATH_VECTOR_TYPE(Float64)

#define ORION_MATH_VECTOR_TYPE(type)            \
	using type##x2 = Detail::Vector2Base<type>; \
	using type##x3 = Detail::Vector3Base<type>; \
	using type##x4 = Detail::Vector4Base<type>;
	ORION_MATH_VECTOR_TYPE_LIST
#undef ORION_MATH_VECTOR_TYPE

	// -- Implementation.
	namespace Detail
	{
		template <typename T>
		constexpr auto Vector2Base<T>::operator[](SizeType index) noexcept -> ReferenceType
		{
			ORION_ASSERT_DEBUG_SLOW(index < 2);
			return _data[index];
		}

		template <typename T>
		constexpr auto Vector2Base<T>::operator[](SizeType index) const noexcept -> ValueType
		{
			ORION_ASSERT_DEBUG_SLOW(index < 2);
			return _data[index];
		}

		template <typename T>
		constexpr auto Vector3Base<T>::operator[](SizeType index) noexcept -> ReferenceType
		{
			ORION_ASSERT_DEBUG_SLOW(index < 3);
			return _data[index];
		}

		template <typename T>
		constexpr auto Vector3Base<T>::operator[](SizeType index) const noexcept -> ValueType
		{
			ORION_ASSERT_DEBUG_SLOW(index < 3);
			return _data[index];
		}

		template <typename T>
		constexpr auto Vector4Base<T>::operator[](SizeType index) noexcept -> ReferenceType
		{
			ORION_ASSERT_DEBUG_SLOW(index < 4);
			return _data[index];
		}

		template <typename T>
		constexpr auto Vector4Base<T>::operator[](SizeType index) const noexcept -> ValueType
		{
			ORION_ASSERT_DEBUG_SLOW(index < 4);
			return _data[index];
		}
	}  // namespace Detail
}  // namespace Orion::Engine::Math
