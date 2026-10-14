#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Traits/TypeComparison.h"

#if defined(ORION_GRAPHICS_VULKAN)
#include "Platform/Graphics/Vulkan/RenderDeviceVk.h"
#endif

#if defined(ORION_GRAPHICS_NONE)
#include "Platform/Graphics/None/RenderDeviceNone.h"
#endif

namespace Orion::Engine::Platform::Graphics
{
	/// @brief Concept that is satisfied by a types that abstract a graphic API under a Render Hardware Interface (RHI).
	template <typename RenderDeviceT>
	concept RenderDeviceKind = requires(RenderDeviceT render_device) {
		{ RenderDeviceT::Create() } -> SameAs<RenderDeviceT*>;
		{ RenderDeviceT::Destroy(&render_device) } -> SameAs<void>;
	};

#if defined(ORION_GRAPHICS_VULKAN)
	static_assert(RenderDeviceKind<Vulkan::RenderDeviceVk>,
	              "RenderDeviceVk does not meet the requirements of being a RenderDevice.");
	using RenderDevice = Vulkan::RenderDeviceVk;
#elif defined(ORION_GRAPHICS_NONE)
	static_assert(RenderDeviceKind<None::RenderDeviceNone>,
	              "RenderDeviceNone does not meet the requirements of being a RenderDevice.");
	using RenderDevice = None::RenderDeviceNone;
#endif
}  // namespace Orion::Engine::Platform::Graphics
