#pragma once

#if defined(ORION_COMPILER_GCC)

namespace Orion::Engine::Platform::Compiler
{
	// NOLINTBEGIN(readability-identifier-naming)
	template <typename T>
	inline constexpr bool IsTriviallyConstructible = __is_trivially_constructible(T);

	template <class T>
	inline constexpr bool IsTriviallyCopyable = __is_trivially_copyable(T);

#if __has_builtin(__is_trivially_destructible)
	template <class T>
	inline constexpr bool IsTriviallyDestructible = __is_trivially_destructible(T);
#elif __has_builtin(__has_trivial_destructor)
	template <class T>
	inline constexpr bool IsTriviallyDestructible = __has_trivial_destructor(T);
#else
#error "__is_trivially_destructible nor __has_trivial_destructor is available for this version of the GCC compiler."
#endif

	template <typename T>
	inline constexpr bool IsEnum = __is_enum(T);

	template <typename T>
	inline constexpr bool IsScopedEnum = __is_scoped_enum(T);

	namespace Detail
	{
		template <typename T>
		struct UnderlyingType
		{
			using Type = __underlying_type(T);
		};
	}  // namespace Detail

	template <typename T>
		requires(IsEnum<T>)
	using UnderlyingType = Detail::UnderlyingType<T>::Type;
	// NOLINTEND(readability-identifier-naming)

	namespace Atomic
	{
		static constexpr UInt8 k_memory_order_relaxed                 = __ATOMIC_RELAXED;
		static constexpr UInt8 k_memory_order_consume                 = __ATOMIC_CONSUME;
		static constexpr UInt8 k_memory_order_acquire                 = __ATOMIC_ACQUIRE;
		static constexpr UInt8 k_memory_order_release                 = __ATOMIC_RELEASE;
		static constexpr UInt8 k_memory_order_acquire_release         = __ATOMIC_ACQ_REL;
		static constexpr UInt8 k_memory_order_sequentially_consistent = __ATOMIC_SEQ_CST;

		template <typename T>
		ORION_FORCE_INLINE void AtomicStore(volatile T* ptr, T desired_value, UInt8 memory_order) noexcept
		{
			__atomic_store_n(ptr, desired_value, memory_order);
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE T AtomicLoad(volatile T* ptr, UInt8 memory_order) noexcept
		{
			return __atomic_load_n(ptr, memory_order);
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE T AtomicExchange(volatile T* ptr, T desired_value, UInt8 memory_order) noexcept
		{
			return __atomic_exchange_n(ptr, desired_value, memory_order);
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchAdd(volatile T* ptr, T desired_value, UInt8 memory_order) noexcept
		{
			return __atomic_fetch_add(ptr, desired_value, memory_order);
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchSub(volatile T* ptr, T desired_value, UInt8 memory_order) noexcept
		{
			return __atomic_fetch_sub(ptr, desired_value, memory_order);
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchAnd(volatile T* ptr, T desired_value, UInt8 memory_order) noexcept
		{
			return __atomic_fetch_and(ptr, desired_value, memory_order);
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchOr(volatile T* ptr, T desired_value, UInt8 memory_order) noexcept
		{
			return __atomic_fetch_or(ptr, desired_value, memory_order);
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchXor(volatile T* ptr, T desired_value, UInt8 memory_order) noexcept
		{
			return __atomic_fetch_xor(ptr, desired_value, memory_order);
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE T AtomicIsLockFree(const volatile T* ptr) noexcept
		{
			return __atomic_is_lock_free(sizeof(T), ptr);
		}
	}  // namespace Atomic

	namespace Memory
	{
		[[nodiscard]] ORION_FORCE_INLINE constexpr void* MemoryAllocate(USize size_in_bytes) noexcept
		{
			return __builtin_malloc(size_in_bytes);
		}

		ORION_FORCE_INLINE constexpr void MemoryFree(void* ptr) noexcept
		{
			return __builtin_free(ptr);
		}

		ORION_FORCE_INLINE constexpr void MemoryCopy(void* dst, const void* src, USize size_in_bytes) noexcept
		{
			__builtin_memcpy(dst, src, size_in_bytes);
		}

		ORION_FORCE_INLINE constexpr void MemoryMove(void* dst, const void* src, USize size_in_bytes) noexcept
		{
			__builtin_memmove(dst, src, size_in_bytes);
		}

		ORION_FORCE_INLINE constexpr void MemorySet(void* dst, Int32 value, USize size_in_bytes) noexcept
		{
			__builtin_memset(dst, value, size_in_bytes);
		}

		ORION_FORCE_INLINE constexpr void MemoryZero(void* dst, USize size_in_bytes) noexcept
		{
			__builtin_memset(dst, 0, size_in_bytes);
		}

		ORION_FORCE_INLINE constexpr int MemoryCompare(const void* lhs, const void* rhs, USize size_in_bytes) noexcept
		{
			return __builtin_memcmp(lhs, rhs, size_in_bytes);
		}
	}  // namespace Memory

	namespace Math
	{
		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE constexpr T Abs(T v) noexcept
		{
			ORION_IGNORE_PARAM(v);
			ORION_NOT_IMPLEMENTED();
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE constexpr T Sin(T v) noexcept
		{
			ORION_IGNORE_PARAM(v);
			ORION_NOT_IMPLEMENTED();
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE constexpr T Cos(T v) noexcept
		{
			ORION_IGNORE_PARAM(v);
			ORION_NOT_IMPLEMENTED();
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE constexpr T Tan(T v) noexcept
		{
			ORION_IGNORE_PARAM(v);
			ORION_NOT_IMPLEMENTED();
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE constexpr T ACos(T v) noexcept
		{
			ORION_IGNORE_PARAM(v);
			ORION_NOT_IMPLEMENTED();
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE constexpr T ASin(T v) noexcept
		{
			ORION_IGNORE_PARAM(v);
			ORION_NOT_IMPLEMENTED();
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE constexpr T ATan(T v) noexcept
		{
			ORION_IGNORE_PARAM(v);
			ORION_NOT_IMPLEMENTED();
		}

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE constexpr T ATan2(T y, T x) noexcept
		{
			ORION_IGNORE_PARAM(y);
			ORION_IGNORE_PARAM(x);
			ORION_NOT_IMPLEMENTED();
		}

		// -- Implementation.
		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 Abs<Float32>(Float32 v) noexcept
		{
			return __builtin_fabsf(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 Abs<Float64>(Float64 v) noexcept
		{
			return __builtin_fabs(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 Sin<Float32>(Float32 v) noexcept
		{
			return __builtin_sinf(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 Sin<Float64>(Float64 v) noexcept
		{
			return __builtin_sin(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 Cos<Float32>(Float32 v) noexcept
		{
			return __builtin_cosf(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 Cos<Float64>(Float64 v) noexcept
		{
			return __builtin_cos(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 Tan<Float32>(Float32 v) noexcept
		{
			return __builtin_tanf(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 Tan<Float64>(Float64 v) noexcept
		{
			return __builtin_tan(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 ACos<Float32>(Float32 v) noexcept
		{
			return __builtin_acosf(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 ACos<Float64>(Float64 v) noexcept
		{
			return __builtin_acos(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 ASin<Float32>(Float32 v) noexcept
		{
			return __builtin_asinf(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 ASin<Float64>(Float64 v) noexcept
		{
			return __builtin_asin(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 ATan<Float32>(Float32 v) noexcept
		{
			return __builtin_atanf(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 ATan<Float64>(Float64 v) noexcept
		{
			return __builtin_atan(v);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float32 ATan2<Float32>(Float32 y, Float32 x) noexcept
		{
			return __builtin_atan2f(y, x);
		}

		template <>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 ATan2<Float64>(Float64 y, Float64 x) noexcept
		{
			return __builtin_atan2(y, x);
		}
	}  // namespace Math
}  // namespace Orion::Engine::Platform::Compiler

#endif
