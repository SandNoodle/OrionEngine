#include "EngineEntry.h"

#include "Core/Console/ConsoleSystem.h"
#include "Core/Log/Logger.h"
#include "Core/Standard/Containers/String.h"
#include "Core/Standard/Containers/StringView.h"
#include "Platform/FileSystem/FileSystem.h"

#include "Platform/Graphics/Vulkan/RenderDeviceVk.h"

namespace Orion::Engine
{
	namespace
	{
		struct CommandLineArguments
		{
			String config_file;
		};
	}  // namespace

	/// @brief Parses CLI arguments while skipping the first argument (Application's executable).
	static constexpr CommandLineArguments ParseCommandLineArguments(int argc, char** argv) noexcept
	{
		USize argument_count = static_cast<USize>(argc);

		CommandLineArguments arguments;
		if (argument_count < 2) {
			return arguments;
		}

		for (USize argument_index = 1; argument_index < argument_count; ++argument_index) {
			if (ORION_STRINGVIEW(argv[argument_index]) == ORION_STRINGVIEW("--config")) {
				arguments.config_file = ORION_STRING(argv[argument_index]);
			}
		}
		return arguments;
	}

	void Initialize(int argc, char** argv) noexcept
	{
		CommandLineArguments command_line_arguments = ParseCommandLineArguments(argc, argv);

		// -------------------------------------------------------------------------------- //
		// Pre-Initialization: Engine Critical Systems
		// -------------------------------------------------------------------------------- //
		Logger::LoggerSystem::Initialize();

		// -------------------------------------------------------------------------------- //
		//
		// -------------------------------------------------------------------------------- //
		//
		// Console::ConsoleSystem::Initialize();

		Platform::FileSystem::FileSystem file_system{};
		Bool8 is_filesystem_initialized = file_system.Initialize();
		ORION_ASSERT(is_filesystem_initialized, "Failed to initialize the FileSystem");

		StringView file = ORION_STRINGVIEW("local://foo.txt");

		Optional<Platform::FileSystem::IOError> create_result = file_system.Create(file);
		ORION_ASSERT(!create_result.IsValue(), "Failed to create a file.");

		Platform::FileSystem::IOResult<Platform::FileSystem::StorageStatInfo> stat_result = file_system.Stat(file);
		ORION_ASSERT(stat_result.IsValue(), "Failed to stat a file.");

		Optional<Platform::FileSystem::IOError> remove_result = file_system.Remove(file);
		ORION_ASSERT(!remove_result.IsValue(), "Failed to remove a file.");

		Platform::FileSystem::IOResult<Platform::FileSystem::StorageStatInfo> post_stat_result = file_system.Stat(file);
		ORION_ASSERT(!post_stat_result.IsValue(), "Failed to stat a file.");

		Bool8 is_filesystem_deinitialized = file_system.Shutdown();
		ORION_ASSERT(is_filesystem_deinitialized, "Failed to shutdown the FileSystem");

		// -------------------------------------------------------------------------------- //
		//
		// -------------------------------------------------------------------------------- //
		Platform::RenderDeviceVk* render_device = Platform::RenderDeviceVk::Create();
		ORION_ASSERT(render_device, "Failed to initialize RenderDevice");

		Platform::RenderDeviceVk::Destroy(render_device);
	}

	void Shutdown() noexcept {}
}  // namespace Orion::Engine
