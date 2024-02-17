#include "core/string/ostring.h"

#include "platform/memory.h"

#include <cstring>

namespace orion
{
	static u64 fnv_hash(const s8* str, size_t length);

	string_t string_t_create(const s8* str)
	{
		string_t s;
		s.length = strlen((char*)str);
		s.data = (s8*)platform_allocate(sizeof(s8) * s.length + 1);
		platform_copy_memory(s.data, str, s.length);
		s.data[s.length] = '\0';
		s.hash = fnv_hash(s.data, s.length);
		return s;
	}

	string_t string_t_duplicate(const string_t* s)
	{
		string_t copy;
		copy.length = s->length;
		copy.hash = s->hash;
		copy.data = (s8*)platform_allocate(sizeof(s8) * copy.length + 1);
		platform_copy_memory(copy.data, s->data, copy.length);
		copy.data[copy.length] = '\0';
		return copy;
	}

	void string_t_destroy(string_t* s)
	{
		if(!s) return;
		s->length = 0;
		platform_free(s->data);
		platform_free(s);
	}

	void string_hash(string_t* s) { s->hash = fnv_hash(s->data, s->length); }

	b8 string_t_equals(const string_t* l, const string_t* r) { return l->hash == r->hash; }

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
