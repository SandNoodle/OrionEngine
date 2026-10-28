#include "Core/Job/JobSystem.h"

#include "Core/Console/ConsoleSystem.h"
#include "Core/Log/Logger.h"
#include "Core/Standard/Math/Math.h"

namespace Orion::Engine::Job
{
	// NOLINTNEXTLINE(readability-identifier-naming)
	static Console::AutoConsoleVariable<Bool8> CVAR_JobSystemInitialized(
		"Engine.Job.Initialized",
		false,
		"Returns true/false depending on if the JobSystem is initialized (or not).",
		"Engine.Job.Initialized <Bool8>",
		Console::ConsoleVariableFlags::Debug | Console::ConsoleVariableFlags::Read);

	// NOLINTNEXTLINE(readability-identifier-naming)
	static Console::AutoConsoleVariable<UInt32> CVAR_WorkerThreadCount(
		"Engine.Job.WorkerThreadCount",
		0UL,
		"Returns the amount of worker threads the JobSystem is using (currently).",
		"Engine.Job.WorkerThreadCount <UInt32>",
		Console::ConsoleVariableFlags::Debug | Console::ConsoleVariableFlags::Read);

	// NOLINTNEXTLINE(readability-identifier-naming)
	static Console::AutoConsoleVariable<UInt32> CVAR_WorkerThreadCountMax(
		"Engine.Job.WorkerThreadCountMax",
		0UL,
		"Returns the maximum amount of worker threads the JobSystem could use (when initializing).",
		"Engine.Job.WorkerThreadCountMax <UInt32>",
		Console::ConsoleVariableFlags::Debug | Console::ConsoleVariableFlags::Read);

	static void JobSystemWorkerThreadFunction(void* argument)
	{
		ORION_IGNORE_PARAM(argument);
		while (true) {
			// TODO(SandNoodle): Function
		}
	}

	Bool8 JobSystem::Initialize() noexcept
	{
		if (CVAR_JobSystemInitialized.Value()) {
			ORION_LOG_ERROR("[JobSystem] Already initialized.");
			return false;
		}

		ORION_LOG_DEBUG("[JobSystem] Initializing...");
		Platform::PlatformInfo platform_info = Platform::GetPlatformInfo();
		UInt32 requested_worker_thread_count = Math::Max<UInt32>(1UL, platform_info.logical_processor_count - 1);

		ORION_LOG_DEBUG("[JobSystem] Requesting initialization of '{}' worker threads.", requested_worker_thread_count);
		_worker_threads.Reserve(requested_worker_thread_count);
		for (USize index = 0; index < requested_worker_thread_count; ++index) {
			Platform::PlatformThreadHandle thread = Platform::ThreadCreate(JobSystemWorkerThreadFunction, nullptr);
			if (!thread) {
				ORION_LOG_WARN("[JobSystem] Failed to initialize a worker thread (index={}).", index);
				continue;
			}
			_worker_threads.Add(thread);
		}
		ORION_LOG_DEBUG("[JobSystem] Initialized '{}' worker threads.", _worker_threads.Size());

		CVAR_WorkerThreadCount.SetValue(static_cast<UInt32>(_worker_threads.Size()));
		CVAR_WorkerThreadCountMax.SetValue(requested_worker_thread_count);
		CVAR_JobSystemInitialized.SetValue(true);

		ORION_LOG_DEBUG("[JobSystem] Initialized.");
		return true;
	}

	Bool8 JobSystem::Shutdown() noexcept
	{
		if (!CVAR_JobSystemInitialized.Value()) {
			ORION_LOG_ERROR("[JobSystem] Already shut down.");
			return false;
		}

		ORION_LOG_DEBUG("[JobSystem] Shutting down.");
		// TODO(SandNoodle): We should wait for threads to finish their jobs and then destroy them.
		for (USize index = 0; index < _worker_threads.Size(); ++index) {
			Platform::ThreadDestroy(_worker_threads[index]);
		}
		_worker_threads.Clear();

		CVAR_WorkerThreadCount.SetValue(0UL);
		CVAR_JobSystemInitialized.SetValue(false);

		ORION_LOG_DEBUG("[JobSystem] Shut down.");
		return true;
	}
}  // namespace Orion::Engine::Job
