#pragma once

#if defined(ORION_GRAPHICS_NONE)

namespace Orion::Engine::Platform::Graphics::None
{
	class RenderDeviceNone
	{
		public:
		using ThisType = RenderDeviceNone;

		public:
		/// @brief TODO
		[[nodiscard]] static ThisType* Create();

		/// @brief TODO
		/// @param[IN, REQUIRED] render_device TODO
		static void Destroy(ThisType* render_device);
	};
}  // namespace Orion::Engine::Platform::Graphics::None

#endif
