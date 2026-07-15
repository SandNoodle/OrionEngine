#include "EngineEntry.h"

#include "Core/Console/ConsoleSystem.h"
#include "Core/Log/Logger.h"
#include "Core/Standard/Containers/Vector.h"
#include "Core/Standard/Types/StringView.h"

namespace Orion::Engine
{
	struct CommandLineArguments
	{
	};

	/// @brief Parses CLI arguments while skipping the first argument (Application's executable).
	Vector<StringView> ParseCommandLineArguments(int argc, char** argv) noexcept
	{
		USize argument_count = static_cast<USize>(argc);

		Vector<StringView> arguments;
		if (argument_count < 2) {
			return arguments;
		}

		arguments.Reserve(argument_count - 1);
		for (USize argument_index = 1; argument_index < argument_count; ++argument_index) {
			arguments.Add(StringView::FromStringLiteral(argv[argument_index]));
		}
		return arguments;
	}

	void Initialize(int argc, char** argv) noexcept
	{
		Vector<StringView> command_line_arguments = ParseCommandLineArguments(argc, argv);

		for (USize index = 0; index < command_line_arguments.Size(); ++index) {
		}

		ConsoleSystem& console_system = ConsoleSystem::Get();
		[[maybe_unused]] ConsoleVariable* console_variable
			= console_system.CreateConsoleVariable<Int32>("Client.Foo", "", 1337, ConsoleVariableFlags::DebugVariable);
	}

	void Shutdown() noexcept {}
}  // namespace Orion::Engine
