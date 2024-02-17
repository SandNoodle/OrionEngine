#pragma once

/**
 * This file contains compile time configuration options for the engine.
 */
namespace orion
{
	//
	// Memory
	//

	#ifndef ORION_MEMORY_DEFAULT_ALIGNMENT
	#define ORION_MEMORY_DEFAULT_ALIGNMENT (2 * sizeof(void*))
	#endif // ORION_DEFAULT_MEMORY_ALIGNMENT
	
	//
	// Graphics API
	//

	#ifndef ORION_GRAPHICS_MAX_SHADER_INPUTS
	#define ORION_GRAPHICS_MAX_SHADER_INPUTS 16
	#endif // ORION_DEFAULT_MEMORY_ALIGNMENT

	#ifndef ORION_GRAPHICS_MAX_TEXTURE_SAMPLERS
	#define ORION_GRAPHICS_MAX_TEXTURE_SAMPLERS 16
	#endif // ORION_GRAPHICS_MAX_SHADER_INPUTS
}

