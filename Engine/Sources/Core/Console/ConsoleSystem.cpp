#include "Core/Console/ConsoleSystem.h"

namespace Orion::Engine
{
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

		CreateConsoleVariable<Float32>("Server.Timescale",
		                               "",
		                               1.0f,
		                               ConsoleVariableFlags::Server | ConsoleVariableFlags::ReadWrite);
	}
}  // namespace Orion::Engine
