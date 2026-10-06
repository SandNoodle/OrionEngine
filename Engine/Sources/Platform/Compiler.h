#pragma once

#include "OrionEngine.h"

#if defined(ORION_COMPILER_CLANG)
#include "Compiler.Clang.h"

#elif defined(ORION_COMPILER_GCC)
#include "Compiler.Gcc.h"

#elif defined(ORION_COMPILER_MSVC)
#include "Compiler.Msvc.h"

#else
#include "Compiler.None.h"
#endif
