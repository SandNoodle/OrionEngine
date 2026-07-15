#pragma once

#include <gtest/gtest.h>

#include "OrionEngine.h"

namespace Orion::Engine::UT
{
	using PrimitiveTypes = ::testing::Types<Bool8,
	                                        Bool16,
	                                        Bool32,
	                                        Bool64,
	                                        UInt8,
	                                        UInt16,
	                                        UInt32,
	                                        UInt64,
	                                        Int8,
	                                        Int16,
	                                        Int32,
	                                        Int64,
	                                        Char8,
	                                        Char16,
	                                        Char32,
	                                        Float32,
	                                        Float64,
	                                        USize,
	                                        ISize>;
}
