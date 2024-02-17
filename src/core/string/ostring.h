#pragma once

#include "platform/types.h"

namespace orion
{
	/** Orion Engine's string class */
	typedef struct string_t string_t;
	struct string_t
	{
		s8* data;
		size_t length;
		u64 hash;
	};

	/** Creates given string_t. */
	string_t string_create(const s8* str);

	/** Duplicates gien string_t. */
	string_t string_duplicate(const string_t* s);

	/** Destroys allocated string_t. */
	void string_destroy(string_t* s);

	/** (Re)Hashes the underlying string. */
	void string_hash(string_t* s);

	/** Compares if two string_ts are equal returns true if they are. */
	b8 string_equals(const string_t* l, const string_t* r);
}
