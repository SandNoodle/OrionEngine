#include "Core/Task/TaskQueue.h"

namespace Orion::Engine::Task
{
	Bool8 TaskQueue::Push(Task* task) noexcept
	{
		ORION_IGNORE_PARAM(task);
		ORION_NOT_IMPLEMENTED();
		return false;
	}

	Task* TaskQueue::Next() noexcept
	{
		ORION_NOT_IMPLEMENTED();
		return nullptr;
	}

	Task* TaskQueue::Steal() noexcept
	{
		ORION_NOT_IMPLEMENTED();
		return nullptr;
	}

	TaskQueue::SizeType TaskQueue::Size() const noexcept
	{
		return _bottom - _top;
	}

	TaskQueue::SizeType TaskQueue::Capacity() const noexcept
	{
		return k_queue_capacity;
	}
}  // namespace Orion::Engine::Task
