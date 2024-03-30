#include "orion.h"

#include "platform/timer.h"

#include "core/assert.h"
#include "core/math/omath.h"

namespace orion
{
	application_t application_t::create(application_desc_t desc, application_callbacks_t callbacks)
	{
		application_t app;
		app._desc      = desc;
		app._callbacks = callbacks;
		app._window    = window_t::create(desc.window_desc);
		return app;
	}

	void application_t::destroy()
	{
		_window.destroy();
	}

	void application_t::start()
	{
		_callbacks.start(_callbacks.user_data);
	}

	void application_t::stop()
	{
		_callbacks.stop(_callbacks.user_data);
	}

	void application_t::run()
	{
		u32 updates_this_second = 0;
		u32 frames_this_second = 0;

		f64 last_time = platform_get_absolute_time();
		f64 timer = last_time;
		f64 accumulator = 0.0f;
		const f64 max_update_time = 1.0f / _desc.updates_per_second;
		while(!_window.get_should_close())
		{
			_window.poll_events();
			f64 current_time = platform_get_absolute_time();
			f64 delta_time = math::min(current_time - last_time, max_update_time);
			last_time = current_time;

			accumulator += delta_time;
			while(accumulator > max_update_time)
			{
				accumulator -= max_update_time;
				updates_this_second++;
				_callbacks.update(max_update_time, _callbacks.user_data);
			}

			frames_this_second++;
			_callbacks.render(_callbacks.user_data);
			_window.swap_buffers();

			if(current_time - timer > 1.0f)
			{
				OE_LOG_TRACE("UPS: %d, FPS: %d, FRAME_TIME: %f ms",
					updates_this_second,
					frames_this_second,
					1000.0f / frames_this_second);
				timer++;
				frames_this_second = 0;
				updates_this_second = 0;
			}
		}
	}
}
