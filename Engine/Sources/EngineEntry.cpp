#include "EngineEntry.h"

#include "Core/Console/ConsoleSystem.h"
#include "Core/Log/Logger.h"
#include "Core/Memory/MemorySystem.h"
#include "Core/Standard/Containers/String.h"
#include "Core/Standard/Containers/StringView.h"

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

		// -------------------------------------------------------------------------------- //
		// Pre-Initialization
		// -------------------------------------------------------------------------------- //
		Memory::MemorySystem_Initialize();

		// -------------------------------------------------------------------------------- //
		//
		// -------------------------------------------------------------------------------- //
		// Logger::LoggerSystem_Initialize();
		//
		// Console::ConsoleSystem_Initialize();
	}

	void Shutdown() noexcept {}
}  // namespace Orion::Engine
