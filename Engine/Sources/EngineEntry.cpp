#include "EngineEntry.h"

#include "Core/Job/JobSystem.h"
#include "Core/Log/Logger.h"
#include "Core/Standard/Containers/String.h"
#include "Core/Standard/Containers/StringView.h"
#include "Platform/FileSystem/FileSystem.h"
#include "Platform/Graphics/RenderDevice.h"

namespace Orion::Engine
{
	namespace
	{
		struct CommandLineArguments
		{
			String config_file;
		};

		class Engine
		{
			public:
			Logger::LoggerSystem logger_system;
			Job::JobSystem job_system;
			Platform::FileSystem::FileSystem file_system;
			Platform::Graphics::RenderDevice render_device;
		};
	}  // namespace

	static Engine* k_engine = nullptr;

	/// @brief Parses CLI arguments while skipping the first argument (Application's executable).
	static constexpr CommandLineArguments ParseCommandLineArguments(int argc, char** argv) noexcept
	{
		CommandLineArguments arguments;
		arguments.config_file = String("OrionEngine.ini");

		USize argument_count = static_cast<USize>(argc);
		if (argument_count < 2) {
			return arguments;
		}

		for (USize argument_index = 1; argument_index < argument_count; ++argument_index) {
			if (StringView(argv[argument_index]) == StringView("--config")) {
				arguments.config_file = String(argv[argument_index]);
			}
		}
		return arguments;
	}

	void Initialize(int argc, char** argv) noexcept
	{
		// -------------------------------------------------------------------------------- //
		// Pre-Initialization: Process command line arguments
		// -------------------------------------------------------------------------------- //
		CommandLineArguments command_line_arguments = ParseCommandLineArguments(argc, argv);

		// -------------------------------------------------------------------------------- //
		// Early Initialization: critical systems & modules.
		// -------------------------------------------------------------------------------- //
		k_engine = new Engine();  // TODO(SandNoodle): @nocheckin

		Bool8 is_logger_initialized = k_engine->logger_system.Initialize();
		ORION_ASSERT(is_logger_initialized, "Failed to initialize the LoggerSystem.");

		Bool8 is_filesystem_initialized = k_engine->file_system.Initialize();
		ORION_ASSERT(is_filesystem_initialized, "Failed to initialize the FileSystem.");

		Bool8 is_job_system_initialized = k_engine->job_system.Initialize();
		ORION_ASSERT(is_job_system_initialized, "Failed to initialize the JobSystem.");

		// -------------------------------------------------------------------------------- //
		// Initialization:
		// -------------------------------------------------------------------------------- //
		Bool8 is_render_device_initialized = k_engine->render_device.Initialize();
		ORION_ASSERT(is_render_device_initialized, "Failed to initialize the RenderDevice.");

		// -------------------------------------------------------------------------------- //
		// Late Initialization:
		// -------------------------------------------------------------------------------- //
	}

	void Shutdown() noexcept
	{
		Bool8 is_render_device_deinitialized = k_engine->render_device.Shutdown();
		ORION_ASSERT(is_render_device_deinitialized, "Failed to shutdown the RenderDevice.");

		Bool8 is_job_system_deinitialized = k_engine->job_system.Shutdown();
		ORION_ASSERT(is_job_system_deinitialized, "Failed to shutdown the JobSystem.");

		Bool8 is_filesystem_deinitialized = k_engine->file_system.Shutdown();
		ORION_ASSERT(is_filesystem_deinitialized, "Failed to shutdown the FileSystem.");

		Bool8 is_logger_deinitialized = k_engine->logger_system.Shutdown();
		ORION_ASSERT(is_logger_deinitialized, "Failed to shutdown the LoggerSystem.");
	}
}  // namespace Orion::Engine
