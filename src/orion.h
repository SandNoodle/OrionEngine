#pragma once

#include "platform/types.h"
#include "core/builds.h"

#include "platform/graphics/window.h"

namespace orion
{
	struct application_callbacks_t
	{
		void* user_data;

		/**
		 * @brief
		 * @param user_data
		 */
		void (*start)(void* user_data);

		/**
		 * @brief
		 * @param user_data
		 */
		void (*stop)(void* user_data);

		/**
		 * @brief
		 * @param delta_time
		 * @param user_data
		 */
		void (*update)(f32 delta_time, void* user_data);

		/**
		 * @brief
		 * @param user_data
		 */
		void (*render)(void* user_data);
	};

	struct application_desc_t
	{
		window_desc_t window_desc;
		u32 updates_per_second;
	};

	struct application_t
	{
		application_desc_t      _desc;
		application_callbacks_t _callbacks;
		window_t                _window;

		static application_t create(application_desc_t desc, application_callbacks_t callbacks);
		void destroy();

		void start();
		void stop();
		void run();
	};
}
