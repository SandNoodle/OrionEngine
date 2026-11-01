#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Containers/Vector.h"
#include "Core/Standard/Threads/Atomic.h"
#include "Platform/Platform.h"

namespace Orion::Engine::Task
{
	enum class TaskSystemState : UInt8
	{
	};

	/// @brief JobSystem is a system responsible for running asynchronous jobs on parallel worker threads.
	class TaskSystem
	{
		public:
		using ThisType = TaskSystem;

		private:
		// TODO(SandNoodle): Refactor Platform::PlatformThreadHandle -> Thread::Thread.
		Vector<Platform::PlatformThreadHandle> _worker_threads{};
		Thread::Atomic<TaskSystemState> _state{};

		public:
		TaskSystem()                         = default;
		TaskSystem(const ThisType&) noexcept = delete;
		TaskSystem(ThisType&&) noexcept      = delete;
		~TaskSystem()                        = default;

		TaskSystem& operator=(const ThisType&) noexcept = delete;
		TaskSystem& operator=(ThisType&&) noexcept      = delete;

		/// @brief Initializes the JobSystem and prepares it for further use. Spawns threads, etc.
		/// @warning Must be called exactly once!
		[[nodiscard]] Bool8 Initialize() noexcept;

		/// @brief Shutdowns the filesystem. Deinitializes all spawned threads.
		/// @warning Must be called exactly once!
		[[nodiscard]] Bool8 Shutdown() noexcept;
	};
}  // namespace Orion::Engine::Task
