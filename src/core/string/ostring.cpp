#include "core/string/ostring.h"

#include "platform/memory.h"

#include <cstring>

namespace orion
{
	static u64 fnv_hash(const s8* str, size_t length);

	string_t string_t::create(const char* str, allocator_t& allocator)
	{
		string_t s;
		s.length = strlen(str);
		s.data = (s8*)allocator.allocate(sizeof(s8) * (s.length + 1), allocator.user_context);
		platform_copy_memory(s.data, str, s.length);
		s.data[s.length] = '\0';
		s.hash = fnv_hash(s.data, s.length);
		return s;
	}

	void string_t::destroy(string_t& str, allocator_t& allocator)
	{
		allocator.deallocate(str.data, str.length * sizeof(s8) + 1, allocator.user_context);
		str.data   = nullptr;
		str.length = 0;
		str.hash   = 0;
	}

	string_t string_t::duplicate(const string_t& str, allocator_t& allocator)
	{
		string_t copy;
		copy.length = str.length;
		copy.hash = str.hash;
		copy.data = (s8*)allocator.allocate(sizeof(s8) * (str.length + 1), allocator.user_context);
		copy.data[copy.length] = '\0';
		platform_copy_memory(copy.data, str.data, str.length);
		return copy;
	}

	u64 string_t::rehash(string_t& str)
	{
		str.hash = fnv_hash(str.data, str.length);
		return str.hash;
	}

	b8 string_t::equals(const string_t& l, const string_t& r)
	{
		return l.hash == r.hash;
	}

	//
	//
	//

	static u64 fnv_hash(const s8* str, size_t length)
	{
		u64 hash = 0xCBF29CE484222325;

		for(size_t i = 0; i < length; ++i)
		{
			hash ^= str[i];
			hash *= 0x00000100000001B3;
		}

		return hash;
	}
}
