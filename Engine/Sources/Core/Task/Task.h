#pragma once

#include "Core/Standard/Containers/Function.h"
#include "Core/Standard/Containers/Vector.h"

namespace Orion::Engine::Task
{
	/// @brief Represents a execution status of a given Job.
	enum class TaskState : UInt8
	{
		/// @brief Job is waiting for worker to pick it up.
		Pending,

		/// @brief Job is still being executed by the worker.
		Running,

		/// @brief Job has finished executing successfully.
		Success,

		/// @brief Job has finished executing with error.
		Error,
	};

	/// @brief Represents a execution priority of a given Job.
	enum class TaskPriority : UInt8
	{
		/// @brief Used for jobs that should only be run occasionally. Example(s):
		Low,

		/// @brief Used for typical jobs. Example(s): gameplay logic, loading assets.
		Normal,

		/// @brief Used for jobs that are critical for engine to run, i.e. ones that should be run ASAP.
		/// Example(s): render, audio, physics, etc.
		High,
	};

	/// @brief TODO
	using TaskFunction = Function<Bool8()>;

	/// @brief TODO
	struct Task
	{
		TaskFunction function{};
		Vector<Task*> dependents;
		TaskPriority priority{ TaskPriority::Normal };
		TaskState state{ TaskState::Pending };
	};
}  // namespace Orion::Engine::Task
