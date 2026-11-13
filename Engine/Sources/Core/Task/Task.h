#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Containers/Function.h"
#include "Core/Standard/Handle.h"

namespace Orion::Engine::Task
{
	/// @brief Represents an execution status of a given Task.
	enum class TaskState : UInt8
	{
		/// @brief Task is waiting for worker to pick it up.
		Pending,

		/// @brief Task is still being executed by the worker.
		Running,

		/// @brief Task has finished executing.
		Finished,
	};

	/// @brief Represents a function to be executed when a Task is being processed.
	using TaskFunction = Function<void()>;

	/// @brief Represents a single `unit` of work.
	ORION_DEFINE_OPAQUE_HANDLE(Task);

	/// @brief Represents a single `unit` of work.
	struct Task
	{
		TaskFunction function{ nullptr };
		TaskState state{ TaskState::Pending };
	};
}  // namespace Orion::Engine::Task
