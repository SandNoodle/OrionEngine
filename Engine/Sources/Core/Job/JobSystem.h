#pragma once

#include "OrionEngine.h"

namespace Orion::Engine::Job
{
	/// @brief TODO
	enum class JobStatus : UInt8
	{
		/// @brief Job is waiting for worker to pick it up.
		Pending,

		/// @brief Job is still being executed by the worker.
		Running,

		/// @brief Job has finished executing.
		Finished,

		/// @brief Job failed to execute.
		Error,
	};

	/// @brief TODO
	enum class JobPriority : UInt8
	{
		Low,
		Normal,
		High,
	};

	struct Job
	{
	};

	/// @brief TODO
	class JobSystem
	{
		public:
		using ThisType = JobSystem;

		public:
		JobSystem()                         = default;
		JobSystem(const ThisType&) noexcept = delete;
		JobSystem(ThisType&&) noexcept      = delete;
		~JobSystem()                        = default;

		JobSystem& operator=(const ThisType&) noexcept = delete;
		JobSystem& operator=(ThisType&&) noexcept      = delete;

		/// @brief Initializes the JobSystem and prepares it for further use. Spawns threads, etc.
		/// @warning Must be called exactly once!
		[[nodiscard]] Bool8 Initialize() noexcept;

		/// @brief Shutdowns the filesystem. Deinitializes all spawned threads.
		/// @warning Must be called exactly once!
		[[nodiscard]] Bool8 Shutdown() noexcept;
	};
}  // namespace Orion::Engine::Job
