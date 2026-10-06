#pragma once

#if defined(ORION_COMPILER_MSVC)

namespace Orion::Engine::Platform::Compiler
{
#error "IsTriviallyConstructible is not implemented for the MSVC compiler."
#error "IsTriviallyCopyable is not implemented for the MSVC compiler."
#error "IsTriviallyDestructible is not implemented for the MSVC compiler."
#error "IsEnum is not implemented for the MSVC compiler."
#error "IsScopedEnum is not implemented for the MSVC compiler."
#error "UnderlyingType is not implemented for the MSVC compiler."

#error "Atomic::k_memory_order_* constexpr constants are not defined for the MSVC compiler."
#error "Atomic::AtomicStore<T> is not implemented for the MSVC compiler."
#error "Atomic::AtomicLoad<T> is not implemented for the MSVC compiler."
#error "Atomic::AtomicExchange<T> is not implemented for the MSVC compiler."
#error "Atomic::AtomicFetchAdd<T> is not implemented for the MSVC compiler."
#error "Atomic::AtomicFetchSub<T> is not implemented for the MSVC compiler."
#error "Atomic::AtomicFetchAnd<T> is not implemented for the MSVC compiler."
#error "Atomic::AtomicFetchOr<T> is not implemented for the MSVC compiler."
#error "Atomic::AtomicFetchXor<T> is not implemented for the MSVC compiler."
#error "Atomic::AtomicIsLockFree<T> is not implemented for the MSVC compiler."

	namespace Memory
	{
		[[nodiscard]] ORION_FORCE_INLINE constexpr void* MemoryAllocate(USize size_in_bytes) noexcept
		{
			return ::malloc(size_in_bytes);
		}

		ORION_FORCE_INLINE constexpr void MemoryFree(void* ptr) noexcept
		{
			return ::free(ptr);
		}

		ORION_FORCE_INLINE constexpr void MemoryCopy(void* dst, const void* src, USize size_in_bytes) noexcept
		{
			::memcpy(dst, src, size_in_bytes);
		}

		ORION_FORCE_INLINE constexpr void MemoryMove(void* dst, const void* src, USize size_in_bytes) noexcept
		{
			::memmove(dst, src, size_in_bytes);
		}

		ORION_FORCE_INLINE constexpr void MemorySet(void* dst, Int32 value, USize size_in_bytes) noexcept
		{
			::memset(dst, value, size_in_bytes);
		}

		ORION_FORCE_INLINE constexpr void MemoryZero(void* dst, USize size_in_bytes) noexcept
		{
			::memset(dst, 0, size_in_bytes);
		}

		ORION_FORCE_INLINE constexpr int MemoryCompare(const void* lhs, const void* rhs, USize size_in_bytes) noexcept
		{
			return ::memcmp(lhs, rhs, size_in_bytes);
		}
	}  // namespace Memory

#error "Math::Abs is not implemented for the MSVC compiler."
#error "Math::Sin is not implemented for the MSVC compiler."
#error "Math::Cos is not implemented for the MSVC compiler."
#error "Math::Tan is not implemented for the MSVC compiler."
#error "Math::ASin is not implemented for the MSVC compiler."
#error "Math::ACos is not implemented for the MSVC compiler."
#error "Math::ATan is not implemented for the MSVC compiler."
#error "Math::ATan2 is not implemented for the MSVC compiler."
}  // namespace Orion::Engine::Platform::Compiler

#endif
