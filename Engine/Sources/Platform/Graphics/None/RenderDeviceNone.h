#pragma once

#if defined(ORION_GRAPHICS_NONE)

namespace Orion::Engine::Platform::Graphics::None
{
	class RenderDeviceNone
	{
		public:
		using ThisType = RenderDeviceNone;

		public:
		/// @brief Initializes the render device and prepares it for further use.
		/// @warning Must be called exactly once!
		[[nodiscard]] Bool8 Initialize() noexcept;

		/// @brief Shutdowns the render device.
		/// @warning Must be called exactly once!
		[[nodiscard]] Bool8 Shutdown() noexcept;
	};
}  // namespace Orion::Engine::Platform::Graphics::None

#endif
