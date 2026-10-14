#if defined(ORION_GRAPHICS_NONE)
#include "Platform/Graphics/None/RenderDeviceNone.h"

#include "Core/Assert.h"

namespace Orion::Engine::Platform::Graphics::None
{
	RenderDeviceNone::ThisType* RenderDeviceNone::Create()
	{
		ORION_NOT_IMPLEMENTED();
		return nullptr;
	}

	void RenderDeviceNone::Destroy(ThisType* render_device)
	{
		ORION_IGNORE_PARAM(render_device);
		ORION_NOT_IMPLEMENTED();
	}
}  // namespace Orion::Engine::Platform::Graphics::None

#endif
