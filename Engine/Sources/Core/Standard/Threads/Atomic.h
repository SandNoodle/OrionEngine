#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Traits/TypeComparison.h"
#include "Core/Standard/Traits/TypeProperties.h"
#include "Platform/Compiler.h"

namespace Orion::Engine
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

	/// @brief TODO
	template <typename T>
	class Atomic
	{
		static_assert(IsTriviallyCopyable<T>                     //
		                  && IsCopyConstructible<T>              //
		                  && IsMoveConstructible<T>              //
		                  && IsCopyAssignable<T>                 //
		                  && IsMoveAssignable<T>                 //
		                  && IsSame<T, RemoveConstVolatile<T>>,  //
		              "Atomic<T> must be trivially copyable, and copy & move constructible and assignable.");

		public:
		using ValueType = T;

		private:
		ValueType _value{};

		public:
		constexpr Atomic() noexcept = default;
		constexpr Atomic(ValueType value) noexcept;
		constexpr Atomic(Atomic&&)      = delete;
		constexpr Atomic(const Atomic&) = delete;

		constexpr Atomic& operator=(const Atomic&)          = delete;
		constexpr Atomic& operator=(const Atomic&) volatile = delete;
		constexpr Atomic& operator=(Atomic&&)               = delete;
		constexpr Atomic& operator=(Atomic&&) volatile      = delete;
		ORION_FORCE_INLINE constexpr ValueType operator=(ValueType value) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator=(ValueType value) volatile noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator+=(ValueType value) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator+=(ValueType value) volatile noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator-=(ValueType value) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator-=(ValueType value) volatile noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator&=(ValueType value) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator&=(ValueType value) volatile noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator|=(ValueType value) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator|=(ValueType value) volatile noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator^=(ValueType value) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator^=(ValueType value) volatile noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator()() const noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator()() const volatile noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator++() noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator++() volatile noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator++(int) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator++(int) volatile noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator--() noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator--() volatile noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator--(int) noexcept;
		ORION_FORCE_INLINE constexpr ValueType operator--(int) volatile noexcept;

		ORION_FORCE_INLINE void Store(ValueType value,
		                              MemoryOrder memory_order = MemoryOrder::SequentiallyConsistent) noexcept;
		ORION_FORCE_INLINE void Store(ValueType value,
		                              MemoryOrder memory_order = MemoryOrder::SequentiallyConsistent) volatile noexcept;

		[[nodiscard]] ORION_FORCE_INLINE ValueType Load(MemoryOrder memory_order
		                                                = MemoryOrder::SequentiallyConsistent) const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType Load(MemoryOrder memory_order
		                                                = MemoryOrder::SequentiallyConsistent) const volatile noexcept;

		[[nodiscard]] ORION_FORCE_INLINE ValueType Exchange(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType Exchange(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;

		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchAdd(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchAdd(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;

		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchSub(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchSub(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;

		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchAnd(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchAnd(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;

		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchOr(ValueType value,
		                                                   MemoryOrder memory_order
		                                                   = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchOr(ValueType value,
		                                                   MemoryOrder memory_order
		                                                   = MemoryOrder::SequentiallyConsistent) volatile noexcept;

		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchXor(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE ValueType FetchXor(ValueType value,
		                                                    MemoryOrder memory_order
		                                                    = MemoryOrder::SequentiallyConsistent) volatile noexcept;
	};

	// -- Free functions.

	/// @brief TODO
	/// @tparam T
	/// @param[IN, REQUIRED] ptr
	/// @param[IN, REQUIRED] desired_value
	/// @param[IN, REQUIRED] memory_order
	template <typename T>
	ORION_FORCE_INLINE void AtomicStore(volatile T* ptr, T desired_value, MemoryOrder memory_order) noexcept
	{
		ORION_ASSERT_DEBUG_SLOW(!(memory_order == MemoryOrder::Consume || memory_order == MemoryOrder::Acquire
		                          || memory_order == MemoryOrder::AcquireRelease));
		Platform::Compiler::Atomic::AtomicStore<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief TODO
	/// @tparam T
	/// @param[IN, REQUIRED] ptr
	/// @param[IN, REQUIRED] memory_order
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicLoad(volatile T* ptr, MemoryOrder memory_order) noexcept
	{
		ORION_ASSERT_DEBUG_SLOW(!(memory_order == MemoryOrder::Release || memory_order == MemoryOrder::AcquireRelease));
		return Platform::Compiler::Atomic::AtomicLoad<T>(ptr, EnumAsUnderlyingType(memory_order));
	}

	/// @brief TODO
	/// @tparam T
	/// @param[IN, REQUIRED] ptr
	/// @param[IN, REQUIRED] desired_value
	/// @param[IN, REQUIRED] memory_order
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicExchange(volatile T* ptr,
	                                                  T desired_value,
	                                                  MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicExchange<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief TODO
	/// @tparam T
	/// @param[IN, REQUIRED] ptr
	/// @param[IN, REQUIRED] desired_value
	/// @param[IN, REQUIRED] memory_order
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchAdd(volatile T* ptr,
	                                                  T desired_value,
	                                                  MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicFetchAdd<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief TODO
	/// @tparam T
	/// @param[IN, REQUIRED] ptr
	/// @param[IN, REQUIRED] desired_value
	/// @param[IN, REQUIRED] memory_order
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchSub(volatile T* ptr,
	                                                  T desired_value,
	                                                  MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicFetchSub<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief TODO
	/// @tparam T
	/// @param[IN, REQUIRED] ptr
	/// @param[IN, REQUIRED] desired_value
	/// @param[IN, REQUIRED] memory_order
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchAnd(volatile T* ptr,
	                                                  T desired_value,
	                                                  MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicFetchAnd<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief TODO
	/// @tparam T
	/// @param[IN, REQUIRED] ptr
	/// @param[IN, REQUIRED] desired_value
	/// @param[IN, REQUIRED] memory_order
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchOr(volatile T* ptr,
	                                                 T desired_value,
	                                                 MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicFetchOr<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief TODO
	/// @tparam T
	/// @param[IN, REQUIRED] ptr
	/// @param[IN, REQUIRED] desired_value
	/// @param[IN, REQUIRED] memory_order
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicFetchXor(volatile T* ptr,
	                                                  T desired_value,
	                                                  MemoryOrder memory_order) noexcept
	{
		return Platform::Compiler::Atomic::AtomicFetchXor<T>(ptr, desired_value, EnumAsUnderlyingType(memory_order));
	}

	/// @brief TODO
	/// @tparam T
	/// @param[IN, REQUIRED] ptr
	template <typename T>
	[[nodiscard]] ORION_FORCE_INLINE T AtomicIsLockFree(const volatile T* ptr) noexcept
	{
		return Platform::Compiler::Atomic::AtomicIsLockFree<T>(sizeof(T), ptr);
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
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator()() const noexcept -> ValueType
	{
		return Load();
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator()() const volatile noexcept -> ValueType
	{
		return Load();
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator++() noexcept -> ValueType
	{
		return FetchAdd(1) + 1;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator++() volatile noexcept -> ValueType
	{
		return FetchAdd(1) + 1;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator++(int) noexcept -> ValueType
	{
		return FetchAdd(1);
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator++(int) volatile noexcept -> ValueType
	{
		return FetchAdd(1);
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator--() noexcept -> ValueType
	{
		return FetchSub(1) - 1;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator--() volatile noexcept -> ValueType
	{
		return FetchSub(1) - 1;
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator--(int) noexcept -> ValueType
	{
		return FetchSub(1);
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr auto Atomic<T>::operator--(int) volatile noexcept -> ValueType
	{
		return FetchSub(1);
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
}  // namespace Orion::Engine
