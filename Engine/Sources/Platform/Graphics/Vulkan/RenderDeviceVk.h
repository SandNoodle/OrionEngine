#pragma once

#if defined(ORION_GRAPHICS_VULKAN)

#include "OrionEngine.h"

#include "volk.h"

namespace Orion::Engine::Platform
{
	class RenderDeviceVk
	{
		private:
		VkInstance _instance              = VK_NULL_HANDLE;
		VkPhysicalDevice _physical_device = VK_NULL_HANDLE;
		VkDevice _logical_device          = VK_NULL_HANDLE;
		VkQueue _queue_graphics           = VK_NULL_HANDLE;
		VkQueue _queue_compute            = VK_NULL_HANDLE;
		VkQueue _queue_transfer           = VK_NULL_HANDLE;

		public:
		RenderDeviceVk(VkInstance instance);
		RenderDeviceVk(const RenderDeviceVk&) noexcept = delete;
		RenderDeviceVk(RenderDeviceVk&&) noexcept      = delete;

		RenderDeviceVk& operator=(const RenderDeviceVk&) noexcept = delete;
		RenderDeviceVk& operator=(RenderDeviceVk&&) noexcept      = delete;

		[[nodiscard]] static RenderDeviceVk* Create();
		static void Destroy(RenderDeviceVk* render_device);
	};
}  // namespace Orion::Engine::Platform

#endif  // ORION_GRAPHICS_VULKAN
