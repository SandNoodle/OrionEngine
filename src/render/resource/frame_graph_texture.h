#pragma once

#include "platform/types.h"

namespace orion
{
	enum frame_graph_texture_type_t : u8
	{
		frame_graph_texture_type_texture_1d,
		frame_graph_texture_type_texture_2d,
		frame_graph_texture_type_texture_3d,
	};

	enum frame_graph_texture_format_t : u8
	{
		// 3 channel
		frame_graph_texture_format_rgb8,
		frame_graph_texture_format_rgb16,
		frame_graph_texture_format_rgb32,

		// 4 channel
		frame_graph_texture_format_rgba8,
		frame_graph_texture_format_rgba16,
		frame_graph_texture_format_rgba32,

		// Depth
		frame_graph_texture_format_depth24stencil8,
	};

	enum frame_graph_texture_filter_t : u8
	{
		frame_graph_texture_filter_nearest,
		frame_graph_texture_filter_linear,
	};

	enum frame_graph_texture_wrap_t : u8
	{
		frame_graph_texture_wrap_repeat,
		frame_graph_texture_wrap_clamp_to_edge,
		frame_graph_texture_wrap_clamp_to_border,
		frame_graph_texture_wrap_mirror_repeat,
		frame_graph_texture_wrap_mirror_clamp_to_edge,
	};

	struct frame_graph_texture_t
	{
		struct desc_t 
		{
			u32 width;
			u32 height;
			frame_graph_texture_type_t   type;
			frame_graph_texture_format_t format;
			frame_graph_texture_filter_t filter;
			frame_graph_texture_wrap_t   wrap;
		};
	};
}
