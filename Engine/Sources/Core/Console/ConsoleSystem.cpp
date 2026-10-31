#include "Core/Console/ConsoleSystem.h"

namespace Orion::Engine::Console
{
	// NOLINTNEXTLINE(readability-identifier-naming)
	static AutoConsoleVariable<String> CVAR_EngineName(
		"Engine.Name",
		String(ORION_ENGINE_NAME),
		"Returns Engine's major version (in the semantic versioning format).",
		"Engine.Name <String>",
		ConsoleVariableFlags::Read);

	// NOLINTNEXTLINE(readability-identifier-naming)
	static AutoConsoleVariable<UInt32> CVAR_EngineVersionMajor(
		"Engine.Version.Major",
		ORION_ENGINE_VERSION_MAJOR,
		"Returns Engine's major version (in the semantic versioning format).",
		"Engine.Version.Major <UInt32>",
		ConsoleVariableFlags::Read);

	// NOLINTNEXTLINE(readability-identifier-naming)
	static AutoConsoleVariable<UInt32> CVAR_EngineVersionMinor(
		"Engine.Version.Major",
		ORION_ENGINE_VERSION_MINOR,
		"Returns Engine's minor version (in the semantic versioning format).",
		"Engine.Version.Major <UInt32>",
		ConsoleVariableFlags::Read);

	// NOLINTNEXTLINE(readability-identifier-naming)
	static AutoConsoleVariable<UInt32> CVAR_EngineVersionPatch(
		"Engine.Version.Major",
		ORION_ENGINE_VERSION_PATCH,
		"Returns Engine's patch version (in the semantic versioning format).",
		"Engine.Version.Major <UInt32>",
		ConsoleVariableFlags::Read);

}  // namespace Orion::Engine::Console
