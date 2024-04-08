#pragma once

#include "platform/types.h"
#include "render/render_pass.h"
#include "orion_config.h"

namespace orion
{
	/**
	 * @brief Frame Graph is @TODO
	 *
	 */
	struct frame_graph_t
	{
		render_pass_t passes[ORION_RENDER_MAX_PASSES_PER_GRAPH]{};
		usize         pass_count = 0;
	};
}
