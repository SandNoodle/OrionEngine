#pragma once

namespace Orion::Engine::Platform
{
	template <typename RenderDeviceImpl>
	class RenderDeviceBase : public RenderDeviceImpl
	{
		public:
		[[nodiscard]] static RenderDeviceImpl* Create();
	};
}  // namespace Orion::Engine::Platform
