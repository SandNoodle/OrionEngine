#include "Core/Console/ConsoleSystem.h"

#include "Core/Standard/MoveAndForward.h"

namespace Orion::Engine
{
	struct ConsoleVariable
	{
		StringView name;
		StringView description;
		UInt32 array_index;
		ConsoleVariableFlags flags;
	};

	struct ConsoleCommand
	{
		StringView name;
		StringView description;
		// TODO(SandNoodle): Implement.
	};

	ConsoleSystem& ConsoleSystem::Get() noexcept
	{
		static ConsoleSystem console_system;
		return console_system;
	}

	ConsoleSystem::ConsoleSystem()
	{
		// -- Builtin ConsoleVariables.
		CreateConsoleVariable<Bool8>("", "", false, static_cast<ConsoleVariableFlags>(ConsoleVariableFlag_Server | ConsoleVariableFlag_ReadWrite));
	}
}  // namespace Orion::Engine
