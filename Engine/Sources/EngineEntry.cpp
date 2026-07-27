#include "EngineEntry.h"

#include "Core/Console/ConsoleSystem.h"
#include "Core/Log/Logger.h"
#include "Core/Standard/Types/String.h"
#include "Core/Standard/Types/StringView.h"

namespace Orion::Engine
{
	struct CommandLineArguments
	{
		String config_file;
	};

	/// @brief Parses CLI arguments while skipping the first argument (Application's executable).
	static constexpr CommandLineArguments ParseCommandLineArguments(int argc, char** argv) noexcept
	{
		USize argument_count = static_cast<USize>(argc);

		CommandLineArguments arguments;
		if (argument_count < 2) {
			return arguments;
		}

		for (USize argument_index = 1; argument_index < argument_count; ++argument_index) {
			if (StringView::Equal(ORION_STRINGVIEW(argv[argument_index]), ORION_STRINGVIEW("--config"))) {
				arguments.config_file = ORION_STRING(argv[argument_index]);
			}
		}
		return arguments;
	}

	void Initialize(int argc, char** argv) noexcept
	{
		CommandLineArguments command_line_arguments = ParseCommandLineArguments(argc, argv);

		ConsoleSystem& console_system = ConsoleSystem::Get();
		[[maybe_unused]] ConsoleVariable* console_variable
			= console_system.CreateConsoleVariable<Int32>("Client.Foo", "", 1337, ConsoleVariableFlags::DebugVariable);
	}

	void Shutdown() noexcept {}
}  // namespace Orion::Engine
