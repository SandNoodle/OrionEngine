#if defined(ORION_GRAPHICS_NONE)
#include "Platform/Graphics/None/RenderDeviceNone.h"

#include "Core/Assert.h"

namespace Orion::Engine::Platform::Graphics::None
{
	Bool8 RenderDeviceNone::Initialize() noexcept
	{
		return true;
	}

	Bool8 RenderDeviceNone::Shutdown() noexcept
	{
		return true;
	}
}  // namespace Orion::Engine::Platform::Graphics::None

#endif
