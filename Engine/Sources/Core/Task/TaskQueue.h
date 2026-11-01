#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Threads/Atomic.h"

namespace Orion::Engine::Task
{
	struct Task;

	/// @brief TODO
	/// @see https://www.dre.vanderbilt.edu/~schmidt/PDF/work-stealing-dequeue.pdf
	class TaskQueue
	{
		public:
		using SizeType = USize;

		static constexpr SizeType k_queue_capacity = 1024;

		private:
		using TaskBuffer = Array<Thread::Atomic<Task*>, k_queue_capacity>;

		private:
		alignas(ORION_PLATFORM_CACHE_SIZE) Thread::Atomic<SizeType> _top{ 0L };
		alignas(ORION_PLATFORM_CACHE_SIZE) Thread::Atomic<SizeType> _bottom{ 0L };
		alignas(ORION_PLATFORM_CACHE_SIZE) TaskBuffer _tasks;

		public:
		/// @brief Attempts to push a given \p task into the queue.
		/// @warning \p task must NOT be nullptr;
		/// @param[IN, REQUIRED] task Task to enqueue.
		/// @returns Returns true if task was successfully pushed on top of the queue, false otherwise.
		[[nodiscard]] Bool8 Push(Task* task) noexcept;

		/// @brief Attempts to acquire the task from the bottom of the queue (in a FIFO manner).
		/// @returns Pointer to a valid task if successful, nullptr otherwise.
		[[nodiscard]] Task* Next() noexcept;

		/// @brief Attempts to steal the task from top of the the queue (in a LIFO manner).
		/// @warning Should be called by the thread attempting to acquire a task, not the one this queue belongs to.
		/// @returns Pointer to a valid task if successful, nullptr otherwise.
		[[nodiscard]] Task* Steal() noexcept;

		/// @brief Returns the amount of tasks that this container currently stores.
		[[nodiscard]] SizeType Size() const noexcept;

		/// @brief Returns the maximum amount of tasks that this container can store.
		[[nodiscard]] SizeType Capacity() const noexcept;
	};
}  // namespace Orion::Engine::Task
