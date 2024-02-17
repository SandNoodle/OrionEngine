#pragma once

namespace orion
{
	struct render_pass_t;

	typedef struct render_graph_t render_graph_t;
	struct render_graph_t
	{
		render_pass_t* render_passes;
		size_t render_passes_count;
	};
}
