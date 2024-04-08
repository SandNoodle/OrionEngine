#pragma once

#include "platform/types.h"
#include "render/resource/frame_graph_resource.h"
#include "orion_config.h"

namespace orion
{
	struct render_pass_t
	{
		frame_graph_resource_id _reads[ORION_RENDER_PASS_MAX_READ_RESOURCES];
		frame_graph_resource_id _writes[ORION_RENDER_PASS_MAX_WRITE_RESOURCES];
		usize                   _reads_count;
		usize                   _writes_count;
	};
}
