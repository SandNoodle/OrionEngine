#pragma once

#include "platform/types.h"

#include "core/memory/allocators/allocator.h"

namespace orion
{
	/** Orion Engine's string class */
	struct string_t
	{
		s8*   data;
		usize length;
		u64   hash;
		
		static string_t create(const char* str, allocator_t& allocator);
		static void     destroy(string_t& str, allocator_t& allocator);
		static string_t duplicate(const string_t& str, allocator_t& allocator);
		static u64      rehash(string_t& str);
		static b8       equals(const string_t& l, const string_t& r);
	};
}
