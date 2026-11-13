#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Threads/Atomic.h"
#include "Core/Task/Task.h"

namespace Orion::Engine::Task
{
	/// @brief TaskQueue is a lock-free non-owning work-stealing deque for processing Tasks; owning (worker) thread push
	/// their tasks at the bottom, while thieving (worker) threads steal from the top (if possible).
	///
	/// Implementation is based on `Dynamic Circular Work-Stealing Deque` (Chase-Lev) and further improved with `Correct
	/// and Efficient Work-Stealing for Weak Memory Models` (Lê et al.).
	/// @see https://www.dre.vanderbilt.edu/~schmidt/PDF/work-stealing-dequeue.pdf
	/// @see https://fzn.fr/readings/ppopp13.pdf
	class TaskQueue
	{
		public:
		using SizeType  = Int64;
		using USizeType = UInt64;

		static constexpr SizeType k_queue_capacity = 1024L;

		private:
		using TaskBuffer = Array<Thread::Atomic<TaskHandle>, k_queue_capacity>;

		private:
		alignas(ORION_PLATFORM_CACHE_SIZE) Thread::Atomic<SizeType> _top{ 0 };
		alignas(ORION_PLATFORM_CACHE_SIZE) Thread::Atomic<SizeType> _bottom{ 0 };
		alignas(ORION_PLATFORM_CACHE_SIZE) TaskBuffer _tasks{ nullptr };

		public:
		TaskQueue()                          = default;
		TaskQueue(const TaskQueue&) noexcept = delete;
		TaskQueue(TaskQueue&&) noexcept      = delete;
		~TaskQueue()                         = default;

		TaskQueue& operator=(const TaskQueue&) noexcept = delete;
		TaskQueue& operator=(TaskQueue&&) noexcept      = delete;

		/// @brief Attempts to push a given \p task into the queue.
		/// @warning \p task must NOT be nullptr;
		/// @param[IN, REQUIRED] task Task to enqueue.
		/// @returns Returns true if task was successfully pushed on top of the queue, false otherwise.
		[[nodiscard]] Bool8 Push(TaskHandle task) noexcept;

		/// @brief Attempts to acquire the task from the bottom of the queue (in a FIFO manner).
		/// @returns Pointer to a valid task if successful, nullptr otherwise.
		[[nodiscard]] TaskHandle Next() noexcept;

		/// @brief Attempts to steal the task from top of the queue (in a LIFO manner).
		/// @warning Should be called by the thread attempting to acquire a task, not the one this queue belongs to.
		/// @returns Pointer to a valid task if successful, nullptr otherwise.
		[[nodiscard]] TaskHandle Steal() noexcept;

		/// @brief Returns the amount of tasks that this container currently stores.
		[[nodiscard]] USizeType Size() const noexcept;

		/// @brief Returns the maximum amount of tasks that this container can store.
		[[nodiscard]] USizeType Capacity() const noexcept;
	};
}  // namespace Orion::Engine::Task
