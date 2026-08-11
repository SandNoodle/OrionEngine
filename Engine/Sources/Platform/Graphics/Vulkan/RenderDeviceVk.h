#pragma once

#if defined(ORION_GRAPHICS_VULKAN)

#include "OrionEngine.h"

#include "volk.h"

namespace Orion::Engine::Platform
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
		RenderDeviceVk(VkInstance instance,
		               VkPhysicalDevice physical_device,
		               VkDevice logical_device,
		               QueueVk queue_graphics,
		               QueueVk queue_compute,
		               QueueVk queue_transfer) noexcept;
		RenderDeviceVk(const RenderDeviceVk&) noexcept = delete;
		RenderDeviceVk(RenderDeviceVk&&) noexcept      = delete;
		~RenderDeviceVk() noexcept;

		RenderDeviceVk& operator=(const RenderDeviceVk&) noexcept = delete;
		RenderDeviceVk& operator=(RenderDeviceVk&&) noexcept      = delete;

		[[nodiscard]] static RenderDeviceVk* Create();
		static void Destroy(RenderDeviceVk* render_device);
	};
}  // namespace Orion::Engine::Platform

#endif  // ORION_GRAPHICS_VULKAN
