#include "Core/Task/TaskQueue.h"

#include "Core/Task/Task.h"

namespace Orion::Engine::Task
{
	Bool8 TaskQueue::Push(Task* task) noexcept
	{
		ORION_ASSERT_DEBUG(task);
		SizeType bottom_index = _bottom.Load(Thread::MemoryOrder::Relaxed);
		SizeType top_index    = _top.Load(Thread::MemoryOrder::Acquire);
		if (bottom_index - top_index >= k_queue_capacity) {
			return false;
		}

		USizeType slot_index = static_cast<USizeType>(bottom_index % k_queue_capacity);
		_tasks[slot_index].Store(task, Thread::MemoryOrder::Relaxed);
		Thread::AtomicThreadFence(Thread::MemoryOrder::Release);
		_bottom.Store(bottom_index + 1L, Thread::MemoryOrder::Relaxed);
		return true;
	}

	Task* TaskQueue::Next() noexcept
	{
		// NOTE: Thieves must be notified of the (bottom) slot reservation - this prevents owner and thief from both
		//       taking the last task - thus the AtomicThreadFence there.
		SizeType bottom_index = _bottom.Load(Thread::MemoryOrder::Relaxed) - 1L;
		_bottom.Store(bottom_index, Thread::MemoryOrder::Relaxed);
		Thread::AtomicThreadFence(Thread::MemoryOrder::SequentiallyConsistent);
		SizeType top_index = _top.Load(Thread::MemoryOrder::Relaxed);

		if (top_index > bottom_index) {
			// NOTE: Queue is empty - undo the (bottom) slot reservation.
			_bottom.Store(bottom_index + 1L, Thread::MemoryOrder::Relaxed);
			return nullptr;
		}

		USizeType slot_index = static_cast<USizeType>(bottom_index % k_queue_capacity);
		Task* task           = _tasks[slot_index].Load(Thread::MemoryOrder::Relaxed);

		if (top_index != bottom_index) {
			// NOTE: There are 2+ tasks left - no thief can touch this one.
			return task;
		}

		// NOTE: Other thieves (worker threads) may attempt stealing our (last) task, we must race them for it.
		//       If it was successfully stolen, then move top one slot over, otherwise do nothing.
		Bool8 is_steal_prevented = _top.CompareExchangeStrong(
			top_index, top_index + 1L, Thread::MemoryOrder::SequentiallyConsistent, Thread::MemoryOrder::Relaxed);
		_bottom.Store(bottom_index + 1L, Thread::MemoryOrder::Relaxed);
		return is_steal_prevented ? task : nullptr;
	}

	Task* TaskQueue::Steal() noexcept
	{
		// NOTE: this AtomicThreadFence is paired up with the TaskQueue::Next's one. Thief must be notified that the
		// owner will take the task, thus loading the top slot before bottom one.
		SizeType top_index = _top.Load(Thread::MemoryOrder::Acquire);
		Thread::AtomicThreadFence(Thread::MemoryOrder::SequentiallyConsistent);
		SizeType bottom_index = _bottom.Load(Thread::MemoryOrder::Acquire);

		if (top_index >= bottom_index) {
			// NOTE: Queue is empty - there is nothing to steal.
			return nullptr;
		}

		// NOTE: Other thieves (worker threads) may attempt stealing a task we are eyeballing, we must race them for it.
		//       If it was successfully prevented from being stolen, then move top one slot over, otherwise do nothing.
		USizeType slot_index      = static_cast<USizeType>(top_index % k_queue_capacity);
		Task* task                = _tasks[slot_index].Load(Thread::MemoryOrder::Relaxed);
		Bool8 is_steal_successful = _top.CompareExchangeStrong(
			top_index, top_index + 1L, Thread::MemoryOrder::SequentiallyConsistent, Thread::MemoryOrder::Relaxed);
		return is_steal_successful ? task : nullptr;
	}

	TaskQueue::USizeType TaskQueue::Size() const noexcept
	{
		SizeType bottom_index = _bottom.Load(Thread::MemoryOrder::Relaxed);
		SizeType top_index    = _top.Load(Thread::MemoryOrder::Relaxed);
		return static_cast<USizeType>(bottom_index > top_index ? bottom_index - top_index : 0UL);
	}

	TaskQueue::USizeType TaskQueue::Capacity() const noexcept
	{
		return k_queue_capacity;
	}
}  // namespace Orion::Engine::Task
