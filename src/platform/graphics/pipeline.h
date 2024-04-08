#pragma once

#include "platform/types.h"

namespace orion
{
	/**
	 * @brief Represents given graphics API's underlying pipeline object.
	 */
	typedef struct pipeline_t pipeline_t;

	enum class fill_mode : u8
	{
		none,      // Only polygons' vertices are drawn.
		solid,     // Interrior of the polygon is filled.
		wireframe, // Boundary edges of the polygons are drawn.
	};

	enum class cull_mode : u8
	{
		none,           // No culling is done.
		front,          // Only front-facing polygons are culled.
		back,           // Only back-facing polygons are culled.
		front_and_back, // Both front- and back- facing polygons are culled.
	};

	enum class primitive_topology : u8
	{
		triangle_list,
		triangle_strip,
		point_list,
	};

	enum class depth_function : u8
	{
		none,          // Disables depth testing.
		never,         // Depth testing NEVER passes.
		always,        // Depth testing ALWAYS passes.
		less,          // Only depth value LESS than the value stored passes.
		less_equal,    // Only depth value LESS or EQUAL than the value stored passes.
		equal,         // Only depth value EQUAL to the value stored passes.
		not_equal,     // Only depth value NOT EQUAL to the value stored passes.
		greater,       // Only depth value GREATER than the value stored passes.
		greater_equal, // Only depth value GREATER or EQUAL than the value stored passes.
	};


	enum pipeline_type : u8
	{
		graphics,
		compute,
	};

	/**
	 * @brief Descriptor of a pipeline.
	 */
	struct pipeline_desc_t
	{
		pipeline_type type = pipeline_type::graphics;
		struct pipeline_rasterizer_t
		{
			fill_mode          fill_mode                 = fill_mode::solid;
			cull_mode          cull_mode                 = cull_mode::none;
			primitive_topology topology                  = primitive_topology::triangle_list;
			b8                 front_counter_clockwise   = true;
			b8                 scissor_enable            = true;
		} rasterizer;
		struct pipeline_depth_stencil_t
		{
			depth_function depth_function = depth_function::less_equal;
			b8             use_depth_mask = true;
		} depth_stencil;
	};

	struct pipeline_t
	{
		pipeline_desc_t _desc;

		/** @brief Creates new pipeline from given descriptor.
		 * @param desc buffer descriptor.
		 * @return newly created pipeline.
		 */
		static pipeline_t create(const pipeline_desc_t& desc);

		/* Prepares pipeline to use and sets its attributes. */
		void execute() const;
	};
}
