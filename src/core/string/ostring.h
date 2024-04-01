#pragma once

#include "platform/types.h"

#include "core/memory/allocators/allocator.h"

namespace orion
{
	/** Orion Engine's string class */
	struct string_t
	{
		typedef c8    value_type;
		typedef usize size_type;
		typedef u64   hash_type;

		value_type* data;
		size_type   length;
		hash_type   hash;
		
		static string_t  create(const char* str, allocator_t& allocator);
		static void      destroy(string_t& str, allocator_t& allocator);
		static string_t  duplicate(const string_t& str, allocator_t& allocator);
		static hash_type rehash(string_t& str);
		static b8        equals(const string_t& l, const string_t& r);
	};
}
