#pragma once

#include "OrionEngine.h"

#include <math.h>

namespace Orion::Engine
{
	template <typename T>
	struct NumericLimits
	{
		static constexpr bool k_is_specialized = false;

		// clang-format off
		static constexpr T Min() noexcept { return T{}; }
		static constexpr T Max() noexcept { return T{}; }
		static constexpr T Epsilon() noexcept { return T{}; }
		static constexpr T Infinity() noexcept { return T{}; }
		static constexpr T SNaN() noexcept { return T{}; }
		static constexpr T QNaN() noexcept { return T{}; }
		// clang-format on
	};

	template <>
	struct NumericLimits<bool>
	{
		static constexpr bool k_is_specialized = true;

		// clang-format off
		[[nodiscard]] ORION_FORCE_INLINE static constexpr bool Min() noexcept { return false; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr bool Max() noexcept { return true; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr bool Epsilon() noexcept { return false; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr bool Infinity() noexcept { return false; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr bool SNaN() noexcept { return false; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr bool QNaN() noexcept { return false; }
		// clang-format on
	};
	template <>
	struct NumericLimits<UInt8>
	{
		static constexpr bool k_is_specialized = true;

		// clang-format off
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt8 Min() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt8 Max() noexcept { return 0xFF; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt8 Epsilon() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt8 Infinity() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt8 SNaN() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt8 QNaN() noexcept { return 0; }
		// clang-format on
	};

	template <>
	struct NumericLimits<UInt16>
	{
		static constexpr bool k_is_specialized = true;

		// clang-format off
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt16 Min() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt16 Max() noexcept { return 0xFFFF; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt16 Epsilon() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt16 Infinity() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt16 SNaN() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt16 QNaN() noexcept { return 0; }
		// clang-format on
	};

	template <>
	struct NumericLimits<UInt32>
	{
		static constexpr bool k_is_specialized = true;

		// clang-format off
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt32 Min() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt32 Max() noexcept { return 0xFFFFFFFFU; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt32 Epsilon() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt32 Infinity() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt32 SNaN() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt32 QNaN() noexcept { return 0; }
		// clang-format on
	};
	template <>
	struct NumericLimits<UInt64>
	{
		static constexpr bool k_is_specialized = true;

		// clang-format off
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt64 Min() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt64 Max() noexcept { return 0xFFFFFFFFFFFFFFFFULL; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt64 Epsilon() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt64 Infinity() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt64 SNaN() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr UInt64 QNaN() noexcept { return 0; }
		// clang-format on
	};

	template <>
	struct NumericLimits<Int8>
	{
		static constexpr bool k_is_specialized = true;

		// clang-format off
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int8 Min() noexcept { return -128; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int8 Max() noexcept { return 127; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int8 Epsilon() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int8 Infinity() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int8 SNaN() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int8 QNaN() noexcept { return 0; }
		// clang-format on
	};

	template <>
	struct NumericLimits<Int16>
	{
		static constexpr bool k_is_specialized = true;

		// clang-format off
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int16 Min() noexcept { return -32768; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int16 Max() noexcept { return 32767; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int16 Epsilon() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int16 Infinity() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int16 SNaN() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int16 QNaN() noexcept { return 0; }
		// clang-format on
	};

	template <>
	struct NumericLimits<Int32>
	{
		static constexpr bool k_is_specialized = true;

		// clang-format off
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int32 Min() noexcept { return -2147483647 - 1; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int32 Max() noexcept { return 2147483647; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int32 Epsilon() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int32 Infinity() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int32 SNaN() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int32 QNaN() noexcept { return 0; }
		// clang-format on
	};

	template <>
	struct NumericLimits<Int64>
	{
		static constexpr bool k_is_specialized = true;

		// clang-format off
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int64 Min() noexcept { return (-9223372036854775807LL - 1); }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int64 Max() noexcept { return 9223372036854775807LL; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int64 Epsilon() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int64 Infinity() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int64 SNaN() noexcept { return 0; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Int64 QNaN() noexcept { return 0; }
		// clang-format on
	};

	template <>
	struct NumericLimits<Float32>
	{
		static constexpr bool k_is_specialized = true;

		// clang-format off
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Float32 Min() noexcept { return -3.40282347e+38F; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Float32 Max() noexcept { return 3.40282347e+38F; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Float32 Epsilon() noexcept { return 1.19209290e-7F; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Float32 Infinity() noexcept
		{
			#if defined (ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
			return __builtin_huge_valf();
			#else
			#error "Float32::Infinity() not defined."
			#endif
		}
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Float32 SNaN() noexcept
		{
			#if defined (ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
			return __builtin_nansf("");
			#else
			#error "Float32::SNaN() not defined."
			#endif
		}
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Float32 QNaN() noexcept
		{
			#if defined (ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
			return __builtin_nanf("");
			#else
			#error "Float32::QNaN() not defined."
			#endif
		}
		// clang-format on
	};

	template <>
	struct NumericLimits<Float64>
	{
		static constexpr bool k_is_specialized = true;

		// clang-format off
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Float64 Min() noexcept { return -1.7976931348623157e+308; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Float64 Max() noexcept { return 1.7976931348623157e+308; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Float64 Epsilon() noexcept { return 2.2204460492503131e-16; }
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Float64 Infinity() noexcept
		{
			#if defined (ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
			return __builtin_huge_val();
			#else
			#error "Float64::Infinity() not defined."
			#endif
		}
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Float64 SNaN() noexcept
		{
			#if defined (ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
			return __builtin_nans("");
			#else
			#error "Float64::SNaN() not defined."
			#endif
		}
		[[nodiscard]] ORION_FORCE_INLINE static constexpr Float64 QNaN() noexcept
		{
			#if defined (ORION_COMPILER_CLANG) || defined(ORION_COMPILER_GCC)
			return __builtin_nan("");
			#else
			#error "Float64::QNaN() not defined."
			#endif
		}
		// clang-format on
	};
}  // namespace Orion::Engine
