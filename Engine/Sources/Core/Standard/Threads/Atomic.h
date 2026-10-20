#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Traits/TypeComparison.h"
#include "Core/Standard/Traits/TypeProperties.h"
#include "Platform/Compiler.h"

namespace Orion::Engine::Thread
{
	/// @brief Enumeration describing how memory access is to be ordered around an atomic operation.
	/// @see https://en.cppreference.com/cpp/atomic/memory_order
	enum class MemoryOrder : UInt8
	{
		/// @brief Relaxed operation - there are no synchronization or ordering constraints imposed on either reads or
		/// writes.
		Relaxed = Platform::Compiler::Atomic::k_memory_order_relaxed,

		/// @brief Consume operation -  no reads or writes in the current thread dependent on the value currently loaded
		/// can be reordered before this load. Writes to data-dependent variables in other threads that release the same
		/// atomic variable are visible in the current thread.
		Consume = Platform::Compiler::Atomic::k_memory_order_consume,

		/// @brief Acquire operation - no reads or writes in the current thread can be reordered before this load. All
		/// writes in other threads that release the same atomic variable are visible in the current thread.
		Acquire = Platform::Compiler::Atomic::k_memory_order_acquire,

		/// @brief Release operation - no reads or writes in the current thread can be reordered after this store. All
		/// writes in the current thread are visible in other threads that acquire the same atomic variable and writes
		/// that carry a dependency into the atomic variable become visible in other threads that consume the same
		/// atomic.
		Release = Platform::Compiler::Atomic::k_memory_order_release,

		/// @brief Acquire-Release operation - no memory reads or writes in the current thread can be reordered before
		/// the load, nor after the store. All writes in other threads that release the same atomic variable are visible
		/// before the modification and the modification is visible in other threads that acquire the same atomic
		/// variable.
		AcquireRelease = Platform::Compiler::Atomic::k_memory_order_acquire_release,

		/// @brief Sequentially consistent operation - a load operation with this memory orders performs an acquire
		/// operation, store performs a release operation, and read-modify-write performs both acquire operation and a
		/// release operation, plus a single total order exists in which all threads observe modifications in the same
		/// order.
		SequentiallyConsistent = Platform::Compiler::Atomic::k_memory_order_sequentially_consistent,
	};

	/// @brief Represents a synchronization primitive that can be used for reading/writing data between threads without
	/// use of locks.
	/// @tparm T Type of the variable.
	template <typename T>
	class Atomic
	{
		static_assert(IsTriviallyCopyable<T>                     //
		                  && IsCopyConstructible<T>              //
		                  && IsMoveConstructible<T>              //
		                  && IsCopyAssignable<T>                 //
		                  && IsMoveAssignable<T>                 //
		                  && IsSame<T, RemoveConstVolatile<T>>,  //
		              "Atomic<T> must be trivially copyable, and copy & move constructible/assignable.");

		public:
		using ValueType = T;

		private:
		ValueType _value{};

		public:
		constexpr Atomic() noexcept = default;
		constexpr Atomic(ValueType value) noexcept;
		constexpr Atomic(Atomic&&)      = delete;
		constexpr Atomic(const Atomic&) = delete;
		constexpr ~Atomic()             = default;

		constexpr Atomic& operator=(const Atomic&)          = delete;
		constexpr Atomic& operator=(const Atomic&) volatile = delete;
		constexpr Atomic& operator=(Atomic&&)               = delete;
		constexpr Atomic& operator=(Atomic&&) volatile      = delete;
		ORION_FORCE_INLINE constexpr ValueType operator=(ValueType value) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator=(ValueType value) volatile noexcept;

		/// @brief Atomically loads and returns the current value of the atomic variable.
		/// Equivalent to Load().
		/// @{
		ORION_FORCE_INLINE constexpr operator ValueType() const noexcept;
		ORION_FORCE_INLINE constexpr operator ValueType() const volatile noexcept;
		/// @}

		/// @brief Checks whether the atomic operations on all objects of this type are lock-free.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE Bool8 IsLockFree() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE Bool8 IsLockFree() const volatile noexcept;
		/// @}

		/// @brief Atomically replaces the current value with \p value. Memory is affected according to \p memory_order.
		/// @warning \p memory_order must NOT be one of: Consume, Acquire or AcquireRelease.
		/// @param[IN, REQUIRED] value Desired value to be stored into this atomic variable.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		ORION_FORCE_INLINE void Store(ValueType value,
		                              MemoryOrder memory_order = MemoryOrder::SequentiallyConsistent) noexcept;
		ORION_FORCE_INLINE void Store(ValueType value,
		                              MemoryOrder memory_order = MemoryOrder::SequentiallyConsistent) volatile noexcept;
		/// @}

		/// @brief Atomically loads and returns the current value of the atomic variable. Memory is affected according
		/// to \p memory_order.
		/// @warning \p memory_order must NOT be one of: Release or AcquireRelease.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType Load(MemoryOrder memory_order
		                                                = MemoryOrder::SequentiallyConsistent) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType Load(MemoryOrder memory_order
		                                                = MemoryOrder::SequentiallyConsistent) const volatile noexcept;
		/// @}

		/// @brief Atomically replaces the underlying value with \p value (a read-modify-write operation) and returns
		/// the value of the atomic variable before the call. Memory is affected according to \p memory_order.
		/// @warning \p memory_order must NOT be one of: Release or AcquireRelease.
		/// @param[IN, REQUIRED] value Desired value to be stored into this atomic variable.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType Exchange(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType Exchange(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;
		/// @}

		/// @brief Atomically compares the value representation of `*this` with that of \p expected_value.
		/// If those are bitwise-equal, replaces the former with \p desired_value (performs read-modify-write
		/// operation). Otherwise, after the atomic operation is completed, copy the value loaded during the atomic
		/// compare to \p expected_value (performs load operation).
		/// @see https://en.cppreference.com/cpp/atomic/atomic/compare_exchange
		/// @param[IN, REQUIRED] expected_value Reference to the value expected to be found in the atomic object.
		/// @param[IN, REQUIRED] desired_value The value to be stored in the atomic object if the value is expected.
		/// @param[IN, REQUIRED] success_order Memory synchronization for the read-modify-write operation if the
		/// comparison succeeds.
		/// @param[IN, REQUIRED] failure_order Memory synchronization for the load operation if the comparison fails.
		/// @{
		[[nodiscard]] Bool8 CompareExchangeWeak(ValueType& expected_value,
		                                        ValueType desired_value,
		                                        MemoryOrder success_order,
		                                        MemoryOrder failure_order) noexcept;
		[[nodiscard]] Bool8 CompareExchangeWeak(ValueType& expected_value,
		                                        ValueType desired_value,
		                                        MemoryOrder success_order,
		                                        MemoryOrder failure_order) volatile noexcept;
		/// @}

		/// @brief Atomically compares the value representation of `*this` with that of \p expected_value.
		/// If those are bitwise-equal, replaces the former with \p desired_value (performs read-modify-write
		/// operation). Otherwise, after the atomic operation is completed, copy the value loaded during the atomic
		/// compare to \p expected_value (performs load operation).
		/// @see https://en.cppreference.com/cpp/atomic/atomic/compare_exchange
		/// @param[IN, REQUIRED] expected_value Reference to the value expected to be found in the atomic object.
		/// @param[IN, REQUIRED] desired_value The value to be stored in the atomic object if the value is expected.
		/// @param[IN, REQUIRED] success_order Memory synchronization for the read-modify-write operation if the
		/// comparison succeeds.
		/// @param[IN, REQUIRED] failure_order Memory synchronization for the load operation if the comparison fails.
		/// @{
		Bool8 CompareExchangeStrong(ValueType& expected_value,
		                            ValueType desired_value,
		                            MemoryOrder success_order,
		                            MemoryOrder failure_order) noexcept;
		Bool8 CompareExchangeStrong(ValueType& expected_value,
		                            ValueType desired_value,
		                            MemoryOrder success_order,
		                            MemoryOrder failure_order) volatile noexcept;
		/// @}
	};

	template <ArithmeticKind T>
	class Atomic<T>
	{
		public:
		using ValueType = T;

		private:
		ValueType _value{};

		public:
		constexpr Atomic() noexcept = default;
		constexpr Atomic(ValueType value) noexcept;
		constexpr Atomic(Atomic&&)      = delete;
		constexpr Atomic(const Atomic&) = delete;
		constexpr ~Atomic()             = default;

		constexpr Atomic& operator=(const Atomic&)          = delete;
		constexpr Atomic& operator=(const Atomic&) volatile = delete;
		constexpr Atomic& operator=(Atomic&&)               = delete;
		constexpr Atomic& operator=(Atomic&&) volatile      = delete;
		ORION_FORCE_INLINE constexpr ValueType operator=(ValueType value) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator=(ValueType value) volatile noexcept;

		/// @brief Atomically loads and returns the current value of the atomic variable.
		/// Equivalent to Load().
		/// @{
		ORION_FORCE_INLINE constexpr operator ValueType() const noexcept;
		ORION_FORCE_INLINE constexpr operator ValueType() const volatile noexcept;
		/// @}

		/// @brief Atomically adds \p value to the current value of the atomic variable.
		/// Equivalent to FetchAdd(\p value) + \p value.
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator+=(ValueType value) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator+=(ValueType value) volatile noexcept;
		/// @}

		/// @brief Atomically subtract \p value from the current value of the atomic variable.
		/// Equivalent to FetchSub(\p value) - \p value.
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator-=(ValueType value) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator-=(ValueType value) volatile noexcept;
		/// @}

		/// @brief Atomically pre-increments the current value.
		/// Equivalent to FetchAdd(1) + 1.
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator++() noexcept
			requires(IntegralKind<ValueType>);
		ORION_FORCE_INLINE constexpr ValueType operator++() volatile noexcept
			requires(IntegralKind<ValueType>);
		/// @}

		/// @brief Atomically post-increments the current value.
		/// Equivalent to FetchAdd(1).
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator++(int) noexcept
			requires(IntegralKind<ValueType>);
		ORION_FORCE_INLINE constexpr ValueType operator++(int) volatile noexcept
			requires(IntegralKind<ValueType>);
		/// @}

		/// @brief Atomically pre-decrements the current value.
		/// Equivalent to FetchSub(1) - 1.
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator--() noexcept
			requires(IntegralKind<ValueType>);
		ORION_FORCE_INLINE constexpr ValueType operator--() volatile noexcept
			requires(IntegralKind<ValueType>);
		/// @}

		/// @brief Atomically post-decrements the current value.
		/// Equivalent to FetchSub(1).
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator--(int) noexcept
			requires(IntegralKind<ValueType>);
		ORION_FORCE_INLINE constexpr ValueType operator--(int) volatile noexcept
			requires(IntegralKind<ValueType>);
		/// @}

		/// @brief Atomically performs bitwise AND between \p value and the value stored in the atomic.
		/// Equivalent to FetchAnd(\p value) & \p value.
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator&=(ValueType value) noexcept
			requires(IntegralKind<ValueType>);
		ORION_FORCE_INLINE constexpr ValueType operator&=(ValueType value) volatile noexcept
			requires(IntegralKind<ValueType>);
		/// @}

		/// @brief Atomically performs bitwise OR between \p value and the value stored in the atomic.
		/// Equivalent to FetchOr(\p value) | \p value.
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator|=(ValueType value) noexcept
			requires(IntegralKind<ValueType>);
		ORION_FORCE_INLINE constexpr ValueType operator|=(ValueType value) volatile noexcept
			requires(IntegralKind<ValueType>);
		/// @}

		/// @brief Atomically performs bitwise XOR between \p value and the value stored in the atomic.
		/// Equivalent to FetchXor(\p value) ^ \p value.
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator^=(ValueType value) noexcept
			requires(IntegralKind<ValueType>);
		ORION_FORCE_INLINE constexpr ValueType operator^=(ValueType value) volatile noexcept
			requires(IntegralKind<ValueType>);
		/// @}

		/// @brief Checks whether the atomic operations on all objects of this type are lock-free.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE Bool8 IsLockFree() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE Bool8 IsLockFree() const volatile noexcept;
		/// @}

		/// @brief Atomically replaces the current value with \p value. Memory is affected according to \p memory_order.
		/// @warning \p memory_order must NOT be one of: Consume, Acquire or AcquireRelease.
		/// @param[IN, REQUIRED] value Desired value to be stored into this atomic variable.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		ORION_FORCE_INLINE void Store(ValueType value,
		                              MemoryOrder memory_order = MemoryOrder::SequentiallyConsistent) noexcept;
		ORION_FORCE_INLINE void Store(ValueType value,
		                              MemoryOrder memory_order = MemoryOrder::SequentiallyConsistent) volatile noexcept;
		/// @}

		/// @brief Atomically loads and returns the current value of the atomic variable. Memory is affected according
		/// to \p memory_order.
		/// @warning \p memory_order must NOT be one of: Release or AcquireRelease.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType Load(MemoryOrder memory_order
		                                                = MemoryOrder::SequentiallyConsistent) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType Load(MemoryOrder memory_order
		                                                = MemoryOrder::SequentiallyConsistent) const volatile noexcept;
		/// @}

		/// @brief Atomically replaces the underlying value with \p value (a read-modify-write operation) and returns
		/// the value of the atomic variable before the call. Memory is affected according to \p memory_order.
		/// @warning \p memory_order must NOT be one of: Release or AcquireRelease.
		/// @param[IN, REQUIRED] value Desired value to be stored into this atomic variable.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType Exchange(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType Exchange(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;
		/// @}

		/// @brief Atomically compares the value representation of `*this` with that of \p expected_value.
		/// If those are bitwise-equal, replaces the former with \p desired_value (performs read-modify-write
		/// operation). Otherwise, after the atomic operation is completed, copy the value loaded during the atomic
		/// compare to \p expected_value (performs load operation).
		/// @see https://en.cppreference.com/cpp/atomic/atomic/compare_exchange
		/// @param[IN, REQUIRED] expected_value Reference to the value expected to be found in the atomic object.
		/// @param[IN, REQUIRED] desired_value The value to be stored in the atomic object if the value is expected.
		/// @param[IN, REQUIRED] success_order Memory synchronization for the read-modify-write operation if the
		/// comparison succeeds.
		/// @param[IN, REQUIRED] failure_order Memory synchronization for the load operation if the comparison fails.
		/// @{
		[[nodiscard]] Bool8 CompareExchangeWeak(ValueType& expected_value,
		                                        ValueType desired_value,
		                                        MemoryOrder success_order,
		                                        MemoryOrder failure_order) noexcept;
		[[nodiscard]] Bool8 CompareExchangeWeak(ValueType& expected_value,
		                                        ValueType desired_value,
		                                        MemoryOrder success_order,
		                                        MemoryOrder failure_order) volatile noexcept;
		/// @}

		/// @brief Atomically compares the value representation of `*this` with that of \p expected_value.
		/// If those are bitwise-equal, replaces the former with \p desired_value (performs read-modify-write
		/// operation). Otherwise, after the atomic operation is completed, copy the value loaded during the atomic
		/// compare to \p expected_value (performs load operation).
		/// @see https://en.cppreference.com/cpp/atomic/atomic/compare_exchange
		/// @param[IN, REQUIRED] expected_value Reference to the value expected to be found in the atomic object.
		/// @param[IN, REQUIRED] desired_value The value to be stored in the atomic object if the value is expected.
		/// @param[IN, REQUIRED] success_order Memory synchronization for the read-modify-write operation if the
		/// comparison succeeds.
		/// @param[IN, REQUIRED] failure_order Memory synchronization for the load operation if the comparison fails.
		/// @{
		Bool8 CompareExchangeStrong(ValueType& expected_value,
		                            ValueType desired_value,
		                            MemoryOrder success_order,
		                            MemoryOrder failure_order) noexcept;
		Bool8 CompareExchangeStrong(ValueType& expected_value,
		                            ValueType desired_value,
		                            MemoryOrder success_order,
		                            MemoryOrder failure_order) volatile noexcept;
		/// @}

		/// @brief Atomically adds \p value to the value stored in the atomic and returns the previously held value.
		/// @param[IN, REQUIRED] value Value to be added.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchAdd(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchAdd(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;
		/// @}

		/// @brief Atomically subtracts \p value to the value stored in the atomic and returns the previously held
		/// value.
		/// @param[IN, REQUIRED] value Value to be subtracted.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchSub(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchSub(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;
		/// @}

		/// @brief Atomically performs max between \p value and the value stored in the atomic, and returns the
		/// previously held value.
		/// @param[IN, REQUIRED] value Value to be compared.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchMax(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept
			requires(IntegralKind<ValueType>);
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchMax(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept
			requires(IntegralKind<ValueType>);
		/// @}

		/// @brief Atomically performs min between \p value and the value stored in the atomic, and returns the
		/// previously held value.
		/// @param[IN, REQUIRED] value Value to be compared.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchMin(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept
			requires(IntegralKind<ValueType>);
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchMin(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept
			requires(IntegralKind<ValueType>);
		/// @}

		/// @brief Atomically performs bitwise AND between \p value and the value stored in the atomic, and returns the
		/// previously held value.
		/// @param[IN, REQUIRED] value Value to be compared.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchAnd(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept
			requires(IntegralKind<ValueType>);
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchAnd(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept
			requires(IntegralKind<ValueType>);
		/// @}

		/// @brief Atomically performs bitwise OR between \p value and the value stored in the atomic, and returns the
		/// previously held value.
		/// @param[IN, REQUIRED] value Value to be compared.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchOr(ValueType value,
		                                                   MemoryOrder memory_order
		                                                   = MemoryOrder::SequentiallyConsistent) noexcept
			requires(IntegralKind<ValueType>);
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchOr(ValueType value,
		                                                   MemoryOrder memory_order
		                                                   = MemoryOrder::SequentiallyConsistent) volatile noexcept
			requires(!FloatingPointKind<ValueType>);
		/// @}

		/// @brief Atomically performs bitwise XOR between \p value and the value stored in the atomic, and returns the
		/// previously held value.
		/// @param[IN, REQUIRED] value Value to be compared.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchXor(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept
			requires(IntegralKind<ValueType>);
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchXor(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept
			requires(IntegralKind<ValueType>);
		/// @}
	};

	template <typename T>
	class Atomic<T*>
	{
		public:
		using ValueType      = T*;
		using DifferenceType = IntPtr;

		private:
		ValueType _value{ nullptr };

		public:
		constexpr Atomic() noexcept = default;
		constexpr Atomic(ValueType value) noexcept;
		constexpr Atomic(Atomic&&)      = delete;
		constexpr Atomic(const Atomic&) = delete;
		constexpr ~Atomic()             = default;

		constexpr Atomic& operator=(const Atomic&)          = delete;
		constexpr Atomic& operator=(const Atomic&) volatile = delete;
		constexpr Atomic& operator=(Atomic&&)               = delete;
		constexpr Atomic& operator=(Atomic&&) volatile      = delete;
		ORION_FORCE_INLINE constexpr ValueType operator=(ValueType value) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator=(ValueType value) volatile noexcept;

		/// @brief Atomically loads and returns the current value of the atomic variable.
		/// Equivalent to Load().
		/// @{
		ORION_FORCE_INLINE constexpr operator ValueType() const noexcept;
		ORION_FORCE_INLINE constexpr operator ValueType() const volatile noexcept;
		/// @}

		/// @brief Atomically adds \p value to the current value of the atomic variable.
		/// Equivalent to FetchAdd(\p value) + \p value.
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator+=(DifferenceType value) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator+=(DifferenceType value) volatile noexcept;
		/// @}

		/// @brief Atomically subtract \p value from the current value of the atomic variable.
		/// Equivalent to FetchSub(\p value) - \p value.
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator-=(DifferenceType value) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator-=(DifferenceType value) volatile noexcept;
		/// @}

		/// @brief Atomically pre-increments the current value.
		/// Equivalent to FetchAdd(1) + 1.
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator++() noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator++() volatile noexcept;
		/// @}

		/// @brief Atomically post-increments the current value.
		/// Equivalent to FetchAdd(1).
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator++(int) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator++(int) volatile noexcept;
		/// @}

		/// @brief Atomically pre-decrements the current value.
		/// Equivalent to FetchSub(1) - 1.
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator--() noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator--() volatile noexcept;
		/// @}

		/// @brief Atomically post-decrements the current value.
		/// Equivalent to FetchSub(1).
		/// @{
		ORION_FORCE_INLINE constexpr ValueType operator--(int) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator--(int) volatile noexcept;
		/// @}

		/// @brief Checks whether the atomic operations on all objects of this type are lock-free.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE Bool8 IsLockFree() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE Bool8 IsLockFree() const volatile noexcept;
		/// @}

		/// @brief Atomically replaces the current value with \p value. Memory is affected according to \p memory_order.
		/// @warning \p memory_order must NOT be one of: Consume, Acquire or AcquireRelease.
		/// @param[IN, REQUIRED] value Desired value to be stored into this atomic variable.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		ORION_FORCE_INLINE void Store(ValueType value,
		                              MemoryOrder memory_order = MemoryOrder::SequentiallyConsistent) noexcept;
		ORION_FORCE_INLINE void Store(ValueType value,
		                              MemoryOrder memory_order = MemoryOrder::SequentiallyConsistent) volatile noexcept;
		/// @}

		/// @brief Atomically loads and returns the current value of the atomic variable. Memory is affected according
		/// to \p memory_order.
		/// @warning \p memory_order must NOT be one of: Release or AcquireRelease.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType Load(MemoryOrder memory_order
		                                                = MemoryOrder::SequentiallyConsistent) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType Load(MemoryOrder memory_order
		                                                = MemoryOrder::SequentiallyConsistent) const volatile noexcept;
		/// @}

		/// @brief Atomically replaces the underlying value with \p value (a read-modify-write operation) and returns
		/// the value of the atomic variable before the call. Memory is affected according to \p memory_order.
		/// @warning \p memory_order must NOT be one of: Release or AcquireRelease.
		/// @param[IN, REQUIRED] value Desired value to be stored into this atomic variable.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType Exchange(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType Exchange(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;
		/// @}

		/// @brief Atomically compares the value representation of `*this` with that of \p expected_value.
		/// If those are bitwise-equal, replaces the former with \p desired_value (performs read-modify-write
		/// operation). Otherwise, after the atomic operation is completed, copy the value loaded during the atomic
		/// compare to \p expected_value (performs load operation).
		/// @see https://en.cppreference.com/cpp/atomic/atomic/compare_exchange
		/// @param[IN, REQUIRED] expected_value Reference to the value expected to be found in the atomic object.
		/// @param[IN, REQUIRED] desired_value The value to be stored in the atomic object if the value is expected.
		/// @param[IN, REQUIRED] success_order Memory synchronization for the read-modify-write operation if the
		/// comparison succeeds.
		/// @param[IN, REQUIRED] failure_order Memory synchronization for the load operation if the comparison fails.
		/// @{
		[[nodiscard]] Bool8 CompareExchangeWeak(ValueType& expected_value,
		                                        ValueType desired_value,
		                                        MemoryOrder success_order,
		                                        MemoryOrder failure_order) noexcept;
		[[nodiscard]] Bool8 CompareExchangeWeak(ValueType& expected_value,
		                                        ValueType desired_value,
		                                        MemoryOrder success_order,
		                                        MemoryOrder failure_order) volatile noexcept;
		/// @}

		/// @brief Atomically compares the value representation of `*this` with that of \p expected_value.
		/// If those are bitwise-equal, replaces the former with \p desired_value (performs read-modify-write
		/// operation). Otherwise, after the atomic operation is completed, copy the value loaded during the atomic
		/// compare to \p expected_value (performs load operation).
		/// @see https://en.cppreference.com/cpp/atomic/atomic/compare_exchange
		/// @param[IN, REQUIRED] expected_value Reference to the value expected to be found in the atomic object.
		/// @param[IN, REQUIRED] desired_value The value to be stored in the atomic object if the value is expected.
		/// @param[IN, REQUIRED] success_order Memory synchronization for the read-modify-write operation if the
		/// comparison succeeds.
		/// @param[IN, REQUIRED] failure_order Memory synchronization for the load operation if the comparison fails.
		/// @{
		[[nodiscard]] Bool8 CompareExchangeStrong(ValueType& expected_value,
		                                          ValueType desired_value,
		                                          MemoryOrder success_order,
		                                          MemoryOrder failure_order) noexcept;
		[[nodiscard]] Bool8 CompareExchangeStrong(ValueType& expected_value,
		                                          ValueType desired_value,
		                                          MemoryOrder success_order,
		                                          MemoryOrder failure_order) volatile noexcept;
		/// @}

		/// @brief Atomically adds \p value to the value stored in the atomic and returns the previously held value.
		/// @param[IN, REQUIRED] value Value to be added.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchAdd(DifferenceType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchAdd(DifferenceType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;
		/// @}

		/// @brief Atomically subtracts \p value to the value stored in the atomic and returns the previously held
		/// value.
		/// @param[IN, REQUIRED] value Value to be subtracted.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchSub(DifferenceType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchSub(DifferenceType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;
		/// @}

		/// @brief Atomically performs max between \p value and the value stored in the atomic, and returns the
		/// previously held value.
		/// @param[IN, REQUIRED] value Value to be compared.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchMax(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchMax(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;
		/// @}

		/// @brief Atomically performs min between \p value and the value stored in the atomic, and returns the
		/// previously held value.
		/// @param[IN, REQUIRED] value Value to be compared.
		/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
		/// @{
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchMin(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchMin(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;
		/// @}
	};

	// -- Free functions.
	/// @brief Atomically replaces the value at \p ptr with \p value. Memory is affected according to \p memory_order.
	/// @warning \p memory_order must NOT be one of: Consume, Acquire or AcquireRelease.
	/// @tparam T Type of the variable.
	/// @param[IN, REQUIRED] ptr Pointer to a variable to atomically replace.
	/// @param[IN, REQUIRED] desired_value Desired value to be stored into this atomic variable.
	/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
	template <typename T>
	ORION_FORCE_INLINE void AtomicStore(volatile T* ptr, T desired_value, MemoryOrder memory_order) noexcept
	{
		ORION_ASSERT_DEBUG_SLOW(!(memory_order == MemoryOrder::Consume || memory_order == MemoryOrder::Acquire
		                          || memory_order == MemoryOrder::AcquireRelease));
		Platform::Compiler::Atomic::AtomicStore<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief Atomically loads and returns the value at \ptr. Memory is affected according to \p memory_order.
	/// @warning \p memory_order must NOT be one of: Release or AcquireRelease.
	/// @tparam T Type of the variable.
	/// @param[IN, REQUIRED] ptr Pointer to a variable to atomically load.
	/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicLoad(const volatile T* ptr, MemoryOrder memory_order) noexcept
	{
		ORION_ASSERT_DEBUG_SLOW(!(memory_order == MemoryOrder::Release || memory_order == MemoryOrder::AcquireRelease));
		return Platform::Compiler::Atomic::AtomicLoad<T>(ptr, EnumAsUnderlyingType(memory_order));
	}

	/// @brief Atomically replaces the value at \p ptr with \p value (a read-modify-write operation) and returns
	/// the value of the atomic variable before the call. Memory is affected according to \p memory_order.
	/// @warning \p memory_order must NOT be one of: Release or AcquireRelease.
	/// @tparam T Type of the variable.
	/// @param[IN, REQUIRED] ptr Pointer to a variable to atomically exchange.
	/// @param[IN, REQUIRED] desired_value Desired value to be stored into this atomic variable.
	/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicExchange(volatile T* ptr,
	                                                  T desired_value,
	                                                  MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicExchange<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief Atomically adds \p value to the value pointed to by \p ptr and returns the previously held value.
	/// @tparam T Type of the variable.
	/// @param[IN, REQUIRED] ptr Pointer to a variable to atomically fetch add.
	/// @param[IN, REQUIRED] desired_value Desired value to be stored into this atomic variable.
	/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchAdd(volatile T* ptr,
	                                                  T desired_value,
	                                                  MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicFetchAdd<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief Atomically subtracts \p value to the value pointed to by \p ptr and returns the previously held value.
	/// @tparam T Type of the variable.
	/// @param[IN, REQUIRED] ptr Pointer to a variable to atomically fetch sub.
	/// @param[IN, REQUIRED] desired_value Desired value to be stored into this atomic variable.
	/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchSub(volatile T* ptr,
	                                                  T desired_value,
	                                                  MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicFetchSub<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief Atomically performs max between \p value and the value pointed to by \p ptr and returns the previously
	/// held value.
	/// @tparam T Type of the variable.
	/// @param[IN, REQUIRED] ptr Pointer to a variable to atomically fetch max.
	/// @param[IN, REQUIRED] desired_value Desired value to be stored into this atomic variable.
	/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchMax(volatile T* ptr,
	                                                  T desired_value,
	                                                  MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicFetchMax<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief Atomically performs min between \p value and the value pointed to by \p ptr and returns the previously
	/// held value.
	/// @tparam T Type of the variable.
	/// @param[IN, REQUIRED] ptr Pointer to a variable to atomically fetch min.
	/// @param[IN, REQUIRED] desired_value Desired value to be stored into this atomic variable.
	/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchMin(volatile T* ptr,
	                                                  T desired_value,
	                                                  MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicFetchMin<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief Atomically performs bitwise AND between \p value and the value pointed to by \p ptr and returns the
	/// previously held value.
	/// @tparam T Type of the variable.
	/// @param[IN, REQUIRED] ptr Pointer to a variable to atomically fetch and.
	/// @param[IN, REQUIRED] desired_value Desired value to be stored into this atomic variable.
	/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchAnd(volatile T* ptr,
	                                                  T desired_value,
	                                                  MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicFetchAnd<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief Atomically performs bitwise OR between \p value and the value pointed to by \p ptr and returns the
	/// previously held value.
	/// @tparam T Type of the variable.
	/// @param[IN, REQUIRED] ptr Pointer to a variable to atomically fetch or.
	/// @param[IN, REQUIRED] desired_value Desired value to be stored into this atomic variable.
	/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchOr(volatile T* ptr,
	                                                 T desired_value,
	                                                 MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicFetchOr<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief Atomically performs bitwise XOR between \p value and the value pointed to by \p ptr and returns the
	/// previously held value.
	/// @tparam T Type of the variable.
	/// @param[IN, REQUIRED] ptr Pointer to a variable to atomically fetch xor.
	/// @param[IN, REQUIRED] desired_value Desired value to be stored into this atomic variable.
	/// @param[IN, REQUIRED] memory_order Memory order constraints to enforce.

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchXor(volatile T* ptr,
	                                                  T desired_value,
	                                                  MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicFetchXor<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief Checks whether the atomic operations on all objects of this type pointed to by \p ptr are lock-free.
	/// @tparam T Type of the variable.
	/// @param[IN, REQUIRED] ptr Pointer to a variable to atomically fetch xor.
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicIsLockFree(const volatile T* ptr) noexcept
	{
		return Platform::Compiler::Atomic::AtomicIsLockFree<T>(ptr);
	}

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicCompareExchangeWeak(volatile T* ptr,
	                                                             T& expected_value,
	                                                             T desired_value,
	                                                             MemoryOrder success_order,
	                                                             MemoryOrder failure_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicCompareExchangeWeak<T>(ptr,
		                                                                expected_value,
		                                                                desired_value,
		                                                                EnumAsUnderlyingType(success_order),
		                                                                EnumAsUnderlyingType(failure_order));
	}

	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicCompareExchangeStrong(volatile T* ptr,
	                                                               T& expected_value,
	                                                               T desired_value,
	                                                               MemoryOrder success_order,
	                                                               MemoryOrder failure_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicCompareExchangeStrong<T>(ptr,
		                                                                  expected_value,
		                                                                  desired_value,
		                                                                  EnumAsUnderlyingType(success_order),
		                                                                  EnumAsUnderlyingType(failure_order));
	}

	// -- Implementation.
	template <typename T>
	constexpr Atomic<T>::Atomic(ValueType value) noexcept : _value(value)
	{
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator=(ValueType value) noexcept -> ValueType
	{
		Store(value);
		return value;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator=(ValueType value) volatile noexcept -> ValueType
	{
		Store(value);
		return value;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr Atomic<T>::operator ValueType() const noexcept
	{
		return Load();
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr Atomic<T>::operator ValueType() const volatile noexcept
	{
		return Load();
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T>::IsLockFree() const noexcept -> Bool8
	{
		return AtomicIsLockFree(&_value);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T>::IsLockFree() const volatile noexcept -> Bool8
	{
		return AtomicIsLockFree(&_value);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T>::Store(ValueType value, MemoryOrder memory_order) noexcept -> void
	{
		AtomicStore<ValueType>(&_value, value, memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T>::Store(ValueType value, MemoryOrder memory_order) volatile noexcept -> void
	{
		AtomicStore<ValueType>(&_value, value, memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T>::Load(MemoryOrder memory_order) const noexcept -> ValueType
	{
		return AtomicLoad<ValueType>(&_value, memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T>::Load(MemoryOrder memory_order) const volatile noexcept -> ValueType
	{
		return AtomicLoad<ValueType>(&_value, memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T>::Exchange(ValueType value, MemoryOrder memory_order) noexcept -> ValueType
	{
		return AtomicExchange(&_value, value, memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T>::Exchange(ValueType value, MemoryOrder memory_order) volatile noexcept
		-> ValueType
	{
		return AtomicExchange(&_value, value, memory_order);
	}

	template <typename T>
	auto Atomic<T>::CompareExchangeWeak(ValueType& expected_value,
	                                    ValueType desired_value,
	                                    MemoryOrder success_order,
	                                    MemoryOrder failure_order) noexcept -> Bool8
	{
		return AtomicCompareExchangeWeak<T>(&_value, expected_value, desired_value, success_order, failure_order);
	}

	template <typename T>
	auto Atomic<T>::CompareExchangeWeak(ValueType& expected_value,
	                                    ValueType desired_value,
	                                    MemoryOrder success_order,
	                                    MemoryOrder failure_order) volatile noexcept -> Bool8
	{
		return AtomicCompareExchangeWeak<T>(&_value, expected_value, desired_value, success_order, failure_order);
	}

	template <typename T>
	auto Atomic<T>::CompareExchangeStrong(ValueType& expected_value,
	                                      ValueType desired_value,
	                                      MemoryOrder success_order,
	                                      MemoryOrder failure_order) noexcept -> Bool8
	{
		return AtomicCompareExchangeStrong<T>(&_value, expected_value, desired_value, success_order, failure_order);
	}

	template <typename T>
	auto Atomic<T>::CompareExchangeStrong(ValueType& expected_value,
	                                      ValueType desired_value,
	                                      MemoryOrder success_order,
	                                      MemoryOrder failure_order) volatile noexcept -> Bool8
	{
		return AtomicCompareExchangeStrong<T>(&_value, expected_value, desired_value, success_order, failure_order);
	}

	template <ArithmeticKind T>
	constexpr Atomic<T>::Atomic(ValueType value) noexcept : _value(value)
	{
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator=(ValueType value) noexcept -> ValueType
	{
		Store(value);
		return value;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator=(ValueType value) volatile noexcept -> ValueType
	{
		Store(value);
		return value;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr Atomic<T>::operator T() const noexcept
	{
		return Load();
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr Atomic<T>::operator T() const volatile noexcept
	{
		return Load();
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator+=(ValueType value) noexcept -> ValueType
	{
		return FetchAdd(value) + value;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator+=(ValueType value) volatile noexcept -> ValueType
	{
		return FetchAdd(value) + value;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator-=(ValueType value) noexcept -> ValueType
	{
		return FetchSub(value) - value;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator-=(ValueType value) volatile noexcept -> ValueType
	{
		return FetchSub(value) - value;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator++() noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchAdd(1) + 1;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator++() volatile noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchAdd(1) + 1;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator++(int) noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchAdd(1);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator++(int) volatile noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchAdd(1);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator--() noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchSub(1) - 1;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator--() volatile noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchSub(1) - 1;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator--(int) noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchSub(1);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator--(int) volatile noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchSub(1);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator&=(ValueType value) noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchAnd(value) & value;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator&=(ValueType value) volatile noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchAnd(value) & value;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator|=(ValueType value) noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchOr(value) | value;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator|=(ValueType value) volatile noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchOr(value) | value;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator^=(ValueType value) noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchXor(value) ^ value;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator^=(ValueType value) volatile noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return FetchXor(value) ^ value;
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::IsLockFree() const noexcept -> Bool8
	{
		return AtomicIsLockFree<ValueType>(&_value);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::IsLockFree() const volatile noexcept -> Bool8
	{
		return AtomicIsLockFree<ValueType>(&_value);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::Store(ValueType value, MemoryOrder memory_order) noexcept -> void
	{
		AtomicStore<ValueType>(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::Store(ValueType value, MemoryOrder memory_order) volatile noexcept -> void
	{
		AtomicStore<ValueType>(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::Load(MemoryOrder memory_order) const noexcept -> ValueType
	{
		return AtomicLoad<ValueType>(&_value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::Load(MemoryOrder memory_order) const volatile noexcept -> ValueType
	{
		return AtomicLoad<ValueType>(&_value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::Exchange(ValueType value, MemoryOrder memory_order) noexcept -> ValueType
	{
		return AtomicExchange(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::Exchange(ValueType value, MemoryOrder memory_order) volatile noexcept
		-> ValueType
	{
		return AtomicExchange(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	auto Atomic<T>::CompareExchangeWeak(ValueType& expected_value,
	                                    ValueType desired_value,
	                                    MemoryOrder success_order,
	                                    MemoryOrder failure_order) noexcept -> Bool8
	{
		return AtomicCompareExchangeWeak<T>(&_value, expected_value, desired_value, success_order, failure_order);
	}

	template <ArithmeticKind T>
	auto Atomic<T>::CompareExchangeWeak(ValueType& expected_value,
	                                    ValueType desired_value,
	                                    MemoryOrder success_order,
	                                    MemoryOrder failure_order) volatile noexcept -> Bool8
	{
		return AtomicCompareExchangeWeak<T>(&_value, expected_value, desired_value, success_order, failure_order);
	}

	template <ArithmeticKind T>
	auto Atomic<T>::CompareExchangeStrong(ValueType& expected_value,
	                                      ValueType desired_value,
	                                      MemoryOrder success_order,
	                                      MemoryOrder failure_order) noexcept -> Bool8
	{
		return AtomicCompareExchangeStrong<T>(&_value, expected_value, desired_value, success_order, failure_order);
	}

	template <ArithmeticKind T>
	auto Atomic<T>::CompareExchangeStrong(ValueType& expected_value,
	                                      ValueType desired_value,
	                                      MemoryOrder success_order,
	                                      MemoryOrder failure_order) volatile noexcept -> Bool8
	{
		return AtomicCompareExchangeStrong<T>(&_value, expected_value, desired_value, success_order, failure_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchAdd(ValueType value, MemoryOrder memory_order) noexcept -> ValueType
	{
		return AtomicFetchAdd(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchAdd(ValueType value, MemoryOrder memory_order) volatile noexcept
		-> ValueType
	{
		return AtomicFetchAdd(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchSub(ValueType value, MemoryOrder memory_order) noexcept -> ValueType
	{
		return AtomicFetchSub(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchSub(ValueType value, MemoryOrder memory_order) volatile noexcept
		-> ValueType
	{
		return AtomicFetchSub(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchMax(ValueType value, MemoryOrder memory_order) noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return AtomicFetchMax(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchMax(ValueType value, MemoryOrder memory_order) volatile noexcept
		-> ValueType
		requires(IntegralKind<ValueType>)
	{
		return AtomicFetchMax(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchMin(ValueType value, MemoryOrder memory_order) noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return AtomicFetchMin(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchMin(ValueType value, MemoryOrder memory_order) volatile noexcept
		-> ValueType
		requires(IntegralKind<ValueType>)
	{
		return AtomicFetchMin(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchAnd(ValueType value, MemoryOrder memory_order) noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return AtomicFetchAnd(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchAnd(ValueType value, MemoryOrder memory_order) volatile noexcept
		-> ValueType
		requires(IntegralKind<ValueType>)
	{
		return AtomicFetchAnd(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchOr(ValueType value, MemoryOrder memory_order) noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return AtomicFetchOr(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchOr(ValueType value, MemoryOrder memory_order) volatile noexcept -> ValueType
		requires(!FloatingPointKind<ValueType>)
	{
		return AtomicFetchOr(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchXor(ValueType value, MemoryOrder memory_order) noexcept -> ValueType
		requires(IntegralKind<ValueType>)
	{
		return AtomicFetchXor(&_value, value, memory_order);
	}

	template <ArithmeticKind T>
	ORION_FORCE_INLINE auto Atomic<T>::FetchXor(ValueType value, MemoryOrder memory_order) volatile noexcept
		-> ValueType
		requires(IntegralKind<ValueType>)
	{
		return AtomicFetchXor(&_value, value, memory_order);
	}

	template <typename T>
	constexpr Atomic<T*>::Atomic(ValueType value) noexcept : _value(value)
	{
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator=(ValueType value) noexcept -> ValueType
	{
		Store(value);
		return value;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator=(ValueType value) volatile noexcept -> ValueType
	{
		Store(value);
		return value;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr Atomic<T*>::operator T*() const noexcept
	{
		return Load();
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr Atomic<T*>::operator T*() const volatile noexcept
	{
		return Load();
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator+=(DifferenceType value) noexcept -> ValueType
	{
		return FetchAdd(value) + value;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator+=(DifferenceType value) volatile noexcept -> ValueType
	{
		return FetchAdd(value) + value;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator-=(DifferenceType value) noexcept -> ValueType
	{
		return FetchSub(value) - value;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator-=(DifferenceType value) volatile noexcept -> ValueType
	{
		return FetchSub(value) - value;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator++() noexcept -> ValueType
	{
		return FetchAdd(1) + 1;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator++() volatile noexcept -> ValueType
	{
		return FetchAdd(1) + 1;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator++(int) noexcept -> ValueType
	{
		return FetchAdd(1);
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator++(int) volatile noexcept -> ValueType
	{
		return FetchAdd(1);
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator--() noexcept -> ValueType
	{
		return FetchSub(1) - 1;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator--() volatile noexcept -> ValueType
	{
		return FetchSub(1) - 1;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator--(int) noexcept -> ValueType
	{
		return FetchSub(1);
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T*>::operator--(int) volatile noexcept -> ValueType
	{
		return FetchSub(1);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::IsLockFree() const noexcept -> Bool8
	{
		return AtomicIsLockFree(&_value);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::IsLockFree() const volatile noexcept -> Bool8
	{
		return AtomicIsLockFree(&_value);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::Store(ValueType value, MemoryOrder memory_order) noexcept -> void
	{
		AtomicStore<ValueType>(&_value, value, memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::Store(ValueType value, MemoryOrder memory_order) volatile noexcept -> void
	{
		AtomicStore<ValueType>(&_value, value, memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::Load(MemoryOrder memory_order) const noexcept -> ValueType
	{
		return AtomicLoad<ValueType>(&_value, memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::Load(MemoryOrder memory_order) const volatile noexcept -> ValueType
	{
		return AtomicLoad<ValueType>(&_value, memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::Exchange(ValueType value, MemoryOrder memory_order) noexcept -> ValueType
	{
		return AtomicExchange(&_value, value, memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::Exchange(ValueType value, MemoryOrder memory_order) volatile noexcept
		-> ValueType
	{
		return AtomicExchange(&_value, value, memory_order);
	}

	template <typename T>
	auto Atomic<T*>::CompareExchangeWeak(ValueType& expected_value,
	                                     ValueType desired_value,
	                                     MemoryOrder success_order,
	                                     MemoryOrder failure_order) noexcept -> Bool8
	{
		return AtomicCompareExchangeWeak<T>(&_value, expected_value, desired_value, success_order, failure_order);
	}

	template <typename T>
	auto Atomic<T*>::CompareExchangeWeak(ValueType& expected_value,
	                                     ValueType desired_value,
	                                     MemoryOrder success_order,
	                                     MemoryOrder failure_order) volatile noexcept -> Bool8
	{
		return AtomicCompareExchangeWeak<T>(&_value, expected_value, desired_value, success_order, failure_order);
	}

	template <typename T>
	auto Atomic<T*>::CompareExchangeStrong(ValueType& expected_value,
	                                       ValueType desired_value,
	                                       MemoryOrder success_order,
	                                       MemoryOrder failure_order) noexcept -> Bool8
	{
		return AtomicCompareExchangeStrong<T>(&_value, expected_value, desired_value, success_order, failure_order);
	}

	template <typename T>
	auto Atomic<T*>::CompareExchangeStrong(ValueType& expected_value,
	                                       ValueType desired_value,
	                                       MemoryOrder success_order,
	                                       MemoryOrder failure_order) volatile noexcept -> Bool8
	{
		return AtomicCompareExchangeStrong<T>(&_value, expected_value, desired_value, success_order, failure_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::FetchAdd(DifferenceType value, MemoryOrder memory_order) noexcept -> ValueType
	{
		return AtomicFetchAdd<ValueType>(&_value, value * sizeof(ValueType), memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::FetchAdd(DifferenceType value, MemoryOrder memory_order) volatile noexcept
		-> ValueType
	{
		return AtomicFetchAdd<ValueType>(&_value, value * sizeof(ValueType), memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::FetchSub(DifferenceType value, MemoryOrder memory_order) noexcept -> ValueType
	{
		return AtomicFetchSub<ValueType>(&_value, value * sizeof(ValueType), memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::FetchSub(DifferenceType value, MemoryOrder memory_order) volatile noexcept
		-> ValueType
	{
		return AtomicFetchSub<ValueType>(&_value, value * sizeof(ValueType), memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::FetchMax(ValueType value, MemoryOrder memory_order) noexcept -> ValueType
	{
		return AtomicFetchMax<ValueType>(&_value, value, memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::FetchMax(ValueType value, MemoryOrder memory_order) volatile noexcept
		-> ValueType
	{
		return AtomicFetchMax<ValueType>(&_value, value, memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::FetchMin(ValueType value, MemoryOrder memory_order) noexcept -> ValueType
	{
		return AtomicFetchMin<ValueType>(&_value, value, memory_order);
	}

	template <typename T>
	ORION_FORCE_INLINE auto Atomic<T*>::FetchMin(ValueType value, MemoryOrder memory_order) volatile noexcept
		-> ValueType
	{
		return AtomicFetchMin<ValueType>(&_value, value, memory_order);
	}
}  // namespace Orion::Engine::Thread
