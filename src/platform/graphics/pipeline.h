#pragma once

#include "platform/types.h"

namespace orion
{
	/**
	 * @brief Represents given graphics API's underlying pipeline object.
	 */
	typedef struct pipeline_t pipeline_t;

	enum fill_mode_t : u8
	{
		fill_mode_none,      // Only polygons' vertices are drawn.
		fill_mode_solid,     // Interrior of the polygon is filled.
		fill_mode_wireframe, // Boundary edges of the polygons are drawn.
	};

	enum cull_mode_t : u8
	{
		cull_mode_none,           // No culling is done.
		cull_mode_front,          // Only front-facing polygons are culled.
		cull_mode_back,           // Only back-facing polygons are culled.
		cull_mode_front_and_back, // Both front- and back- facing polygons are culled.
	};

	enum primitive_topology_t : u8
	{
		primitive_topology_triangle_list,
		primitive_topology_triangle_strip,
		primitive_topology_point_list,
	};

	enum depth_function_t : u8
	{
		depth_function_none,          // Disables depth testing.
		depth_function_never,         // Depth testing NEVER passes.
		depth_function_always,        // Depth testing ALWAYS passes.
		depth_function_less,          // Only depth value LESS than the value stored passes.
		depth_function_less_equal,    // Only depth value LESS or EQUAL than the value stored passes.
		depth_function_equal,         // Only depth value EQUAL to the value stored passes.
		depth_function_not_equal,     // Only depth value NOT EQUAL to the value stored passes.
		depth_function_greater,       // Only depth value GREATER than the value stored passes.
		depth_function_greater_equal, // Only depth value GREATER or EQUAL than the value stored passes.
	};


	enum pipeline_type_t : u8
	{
		pipeline_type_graphics,
		pipeline_type_compute,
	};

	/**
	 * @brief Descriptor of a pipeline.
	 */
	struct pipeline_desc_t
	{
		pipeline_type_t type = pipeline_type_graphics;
		struct pipeline_rasterizer_t
		{
			fill_mode_t fill_mode = fill_mode_solid;
			cull_mode_t cull_mode = cull_mode_none;
			primitive_topology_t topology = primitive_topology_triangle_list;
			b8 front_counter_clockwise = true;
			b8 scissor_enable = true;
		} rasterizer;
		struct pipeline_depth_stencil_t
		{
			depth_function_t depth_function = depth_function_less_equal;
			b8 use_depth_mask             = true;
		} depth_stencil;
	};

	/** @brief Creates new pipeline from given descriptor.
	 * @param desc buffer descriptor.
	 * @return newly created pipeline.
	 */
	pipeline_t* pipeline_create(pipeline_desc_t desc);

	/**
	 * @brief Destroys given pipeline.
	 * @param buffer pipeline to be destroyed.
	 */
	void pipeline_destroy(pipeline_t* p);

	/* Prepares pipeline to use and sets its attributes. */
	void pipeline_execute(pipeline_t* p);
}
