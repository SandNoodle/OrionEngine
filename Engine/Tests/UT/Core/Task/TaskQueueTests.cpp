#include "TestCommon.h"

#include "Core/Standard/Containers/Vector.h"
#include "Core/Task/Task.h"
#include "Core/Task/TaskQueue.h"

namespace Orion::Engine::Task::UT
{
	namespace
	{
		class TaskQueueTest : public ::testing::Test
		{
			protected:
			TaskQueue _queue;
		};
	}  // namespace

	static Vector<Task> GenerateTasks(USize count, const Function<Bool8()>& function)
	{
		Vector<Task> tasks{};
		tasks.AddZeroed(count);
		for (USize index = 0UL; index < tasks.Size(); ++index) {
			tasks[index].function = function;
		}
		return tasks;
	}

	TEST_F(TaskQueueTest, SingleThread_OwnerPushingAndPulling)
	{
		USize result_value = 0UL;
		Vector<Task> tasks = GenerateTasks(5UL, [&result_value]() -> Bool8 {
			++result_value;
			return true;
		});

		ASSERT_EQ(_queue.Size(), 0UL);
		EXPECT_EQ(_queue.Capacity(), TaskQueue::k_queue_capacity);

		for (USize index = 0; index < tasks.Size(); ++index) {
			Bool8 result = _queue.Push(&tasks[index]);
			ASSERT_TRUE(result) << "Failed to push a task to the TaskQueue.";
		}

		ASSERT_EQ(_queue.Size(), 5UL);
		EXPECT_EQ(_queue.Capacity(), TaskQueue::k_queue_capacity);

		EXPECT_EQ(result_value, 0UL);
		for (USize index = 0; index < tasks.Size(); ++index) {
			Task* task = _queue.Next();
			ASSERT_TRUE(task) << "Failed to pull a task from the TaskQueue.";
			ASSERT_EQ(_queue.Size(), tasks.Size() - index - 1) << "Task was not removed from the TaskQueue.";

			Bool8 execute_result = task->function();
			ASSERT_TRUE(execute_result);
			EXPECT_EQ(result_value, index + 1);
		}

		ASSERT_EQ(_queue.Size(), 0UL);
		EXPECT_EQ(_queue.Capacity(), TaskQueue::k_queue_capacity);
	}

	TEST_F(TaskQueueTest, SingleThread_OwnerPushingAndThiefPulling)
	{
		USize result_value = 0UL;
		Vector<Task> tasks = GenerateTasks(5UL, [&result_value]() -> Bool8 {
			++result_value;
			return true;
		});

		for (USize index = 0; index < tasks.Size(); ++index) {
			Bool8 result = _queue.Push(&tasks[index]);
			ASSERT_TRUE(result) << "Failed to push a task to the TaskQueue.";
		}

		ASSERT_EQ(_queue.Size(), 5UL);
		EXPECT_EQ(_queue.Capacity(), TaskQueue::k_queue_capacity);

		// NOTE: This test is single threaded - there's no 'thief' to speak of, we just simulate one.
		EXPECT_EQ(result_value, 0UL);
		for (USize index = 0; index < tasks.Size(); ++index) {
			Task* task = _queue.Steal();
			ASSERT_TRUE(task) << "Failed to pull (steal) a task from the TaskQueue.";
			ASSERT_EQ(_queue.Size(), tasks.Size() - index - 1) << "Task was not removed from the TaskQueue.";

			Bool8 execute_result = task->function();
			ASSERT_TRUE(execute_result);
			EXPECT_EQ(result_value, index + 1);
		}

		ASSERT_EQ(_queue.Size(), 0UL);
		EXPECT_EQ(_queue.Capacity(), TaskQueue::k_queue_capacity);
	}
}  // namespace Orion::Engine::Task::UT
