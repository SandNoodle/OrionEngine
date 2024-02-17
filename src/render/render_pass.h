#pragma once

#include "platform/types.h"

namespace orion
{
	typedef struct render_pass_t render_pass_t;

	/**
	 * @brief 
	 */
	struct render_pass_t
	{
		b8   (*create) (render_pass_t* self);
		b8   (*execute)(render_pass_t* self);
		void (*destroy)(render_pass_t* self);
	};
}
