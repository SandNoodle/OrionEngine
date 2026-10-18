#pragma once

#if defined(ORION_GRAPHICS_VULKAN)

#include "OrionEngine.h"

#include "volk.h"

namespace Orion::Engine::Platform::Graphics::Vulkan
{
	struct QueueVk
	{
		VkQueue queue{ VK_NULL_HANDLE };
		UInt32 queue_index{ 0UL };
		UInt32 queue_family_index{ 0UL };
	};

	class RenderDeviceVk
	{
		private:
		VkInstance _instance{ VK_NULL_HANDLE };
		VkPhysicalDevice _physical_device{ VK_NULL_HANDLE };
		VkDevice _logical_device{ VK_NULL_HANDLE };
		QueueVk _queue_graphics{};
		QueueVk _queue_compute{};
		QueueVk _queue_transfer{};

		public:
		RenderDeviceVk() noexcept                      = default;
		RenderDeviceVk(const RenderDeviceVk&) noexcept = delete;
		RenderDeviceVk(RenderDeviceVk&&) noexcept      = delete;
		~RenderDeviceVk() noexcept                     = default;

		RenderDeviceVk& operator=(const RenderDeviceVk&) noexcept = delete;
		RenderDeviceVk& operator=(RenderDeviceVk&&) noexcept      = delete;

		/// @brief Initializes the render device and prepares it for further use.
		/// @warning Must be called exactly once!
		[[nodiscard]] Bool8 Initialize() noexcept;

		/// @brief Shutdowns the render device.
		/// @warning Must be called exactly once!
		[[nodiscard]] Bool8 Shutdown() noexcept;
	};
}  // namespace Orion::Engine::Platform::Graphics::Vulkan

#endif  // ORION_GRAPHICS_VULKAN
