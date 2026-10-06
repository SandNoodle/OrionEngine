#pragma once

#if defined(ORION_COMPILER_NONE)

namespace Orion::Engine::Platform::Compiler
{
#error "IsTriviallyConstructible is not implemented for this compiler."
#error "IsTriviallyCopyable is not implemented for this compiler."
#error "IsTriviallyDestructible is not implemented for this compiler."
#error "IsEnum is not implemented for this compiler."
#error "IsScopedEnum is not implemented for this compiler."
#error "UnderlyingType is not implemented for this compiler."

#error "Atomic::k_memory_order_* constexpr constants are not defined for this compiler."
#error "Atomic::AtomicStore<T> is not implemented for this compiler."
#error "Atomic::AtomicLoad<T> is not implemented for this compiler."
#error "Atomic::AtomicExchange<T> is not implemented for this compiler."
#error "Atomic::AtomicFetchAdd<T> is not implemented for this compiler."
#error "Atomic::AtomicFetchSub<T> is not implemented for this compiler."
#error "Atomic::AtomicFetchAnd<T> is not implemented for this compiler."
#error "Atomic::AtomicFetchOr<T> is not implemented for this compiler."
#error "Atomic::AtomicFetchXor<T> is not implemented for this compiler."
#error "Atomic::AtomicIsLockFree<T> is not implemented for this compiler."

#error "Memory::MemoryAllocate is not implemented for this compiler."
#error "Memory::MemoryFree is not implemented for this compiler."
#error "Memory::MemoryCopy is not implemented for this compiler."
#error "Memory::MemoryMove is not implemented for this compiler."
#error "Memory::MemorySet is not implemented for this compiler."
#error "Memory::MemoryZero is not implemented for this compiler."
#error "Memory::MemoryCompare is not implemented for this compiler."

#error "Math::Abs is not implemented for this compiler."
#error "Math::Sin is not implemented for this compiler."
#error "Math::Cos is not implemented for this compiler."
#error "Math::Tan is not implemented for this compiler."
#error "Math::ASin is not implemented for this compiler."
#error "Math::ACos is not implemented for this compiler."
#error "Math::ATan is not implemented for this compiler."
#error "Math::ATan2 is not implemented for this compiler."
}  // namespace Orion::Engine::Platform::Compiler

#endif
