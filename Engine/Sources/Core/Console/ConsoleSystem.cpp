#include "Core/Console/ConsoleSystem.h"

namespace Orion::Engine::Console
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
			"Server.Cheats <Bool8>",
			false,
			ConsoleVariableFlags::Server | ConsoleVariableFlags::ReadWrite);

		CreateConsoleVariable<Float64>("Server.Timescale",
		                               "Speed at which the simulation's update loop happens. Effectively enables "
		                               "speeding up or slowing down the game's speed.",
		                               "Server.Timescale <Float64>",
		                               1.0,
		                               ConsoleVariableFlags::Server | ConsoleVariableFlags::ReadWrite);
	}
}  // namespace Orion::Engine::Console
