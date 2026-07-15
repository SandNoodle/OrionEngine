#include "Core/Console/ConsoleSystem.h"

#include "Core/Standard/MoveAndForward.h"

namespace Orion::Engine
{
	struct ConsoleVariable
	{
		StringView name;
		StringView description;
		StringView usage;
		UInt32 array_index;
		ConsoleVariableFlags flags;
	};

	struct ConsoleCommand
	{
		StringView name;
		StringView description;
		StringView usage;
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
		CreateConsoleVariable<Bool8>(
			"Server.Cheats",
			"Enables the use of console variables and commands that might be considered 'cheating' in a normal play.",
			false,
			ConsoleVariableFlags::Server | ConsoleVariableFlags::ReadWrite);
	}
}  // namespace Orion::Engine
