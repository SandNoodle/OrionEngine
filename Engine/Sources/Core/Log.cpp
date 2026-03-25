#include "Core/Log.h"

#include "quill/LogMacros.h"
#include "quill/Logger.h"

namespace Orion::Engine::Log
{
	namespace Detail
	{
		extern quill::Logger* k_logger;
	}

	void Initialize() noexcept
	{
		k_logger = quill::
	}

	void Shutdown() noexcept {}
}  // namespace Orion::Engine::Log
