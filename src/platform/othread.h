#pragma once

#include "platform.h"
#include "platform/types.h"

namespace orion
{
	typedef struct thread_t thread_t;
	struct thread_t
	{
		u64 id;
		void* internal_data;
	};

	/** */
	b8 thread_create(thread_t* thread);

	/** */
	void thread_destroy(thread_t* thread);
}
