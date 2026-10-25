#pragma once

/// @brief Defines opaque handle to some structure. Useful for hiding implementation details.
/// @param[IN, REQUIRED] type Type for which handle will get generated.
#define ORION_DEFINE_OPAQUE_HANDLE(type) typedef struct type* type##Handle;
