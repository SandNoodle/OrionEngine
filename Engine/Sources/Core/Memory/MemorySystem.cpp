#include "Core/Memory/MemorySystem.h"

#include "OrionEngine.h"

namespace Orion::Engine::Memory
{
	// NOTE: MemorySystem is the first system to be initialized in the engine's lifecycle.
	// This means that, we cannot use ORION_ASSERT macros, as they depend on this system to be up and running to do any
	// logging. To avoid this circular dependency, we define our own custom macro, which will simply crash the engine
	// when triggered. This is acceptable, as at this point we cannot do any meaningful recovery.
#define ORION_ASSERT_MEMORY(condition) \
	do {                               \
		if (!(condition)) {            \
			ORION_BUILTIN_TRAP();      \
		}                              \
	} while (false)

	static Bool8 is_memory_system_initialized = false;

	void MemorySystem_Initialize() noexcept
	{
		ORION_ASSERT_MEMORY(!is_memory_system_initialized);
		is_memory_system_initialized = true;
	}

	void MemorySystem_Shutdown() noexcept
	{
		ORION_ASSERT_MEMORY(is_memory_system_initialized);
		is_memory_system_initialized = false;
	}
}  // namespace Orion::Engine::Memory
