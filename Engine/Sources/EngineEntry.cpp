#include "EngineEntry.h"

#include "Core/Log.h"
#include "Core/Console/ConsoleSystem.h"
#include "Core/Standard/Containers/Vector.h"
#include "Core/Standard/Strings/StringView.h"
#include "Platform/Platform.h"

#include <string.h>

namespace Orion::Engine
{
	struct CommandLineArguments
	{
	};

	/// @brief Parses CLI arguments while skipping the first argument (Application's executable).
	Vector<StringView> ParseCommandLineArguments(int argc, char** argv) noexcept
	{
		Vector<StringView> arguments;
		if (argc < 2) {
			return arguments;
		}

		arguments.Reserve(argc - 1);
		for (USize argument_index = 1; argument_index < argc; ++argument_index) {
			arguments.AddConstruct(argv[argument_index], strlen(argv[argument_index]));
		}
		return arguments;
	}

	void Initialize(int argc, char** argv) noexcept
	{
		// Stage: Pre-Initialize
		Log::Initialize();

		// Stage: Initialize
		Vector<StringView> command_line_arguments = ParseCommandLineArguments(argc, argv);

		// Stage: Post-Initialize
	}

	void Shutdown() noexcept {
	}
}  // namespace Orion::Engine
