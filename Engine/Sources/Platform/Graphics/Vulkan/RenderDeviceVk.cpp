#if defined(ORION_GRAPHICS_VULKAN)
#include "Platform/Graphics/Vulkan/RenderDeviceVk.h"

#include "Core/Log/Logger.h"
#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Containers/Span.h"
#include "Core/Standard/Containers/Vector.h"
#include "Core/Standard/Limits.h"

#define VOLK_IMPLEMENTATION
#include "volk.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Orion::Engine::Platform
{
	static constexpr UInt32 k_vulkan_api_version = VK_API_VERSION_1_3;

	RenderDeviceVk::RenderDeviceVk(VkInstance instance,
	                               VkPhysicalDevice physical_device,
	                               VkDevice logical_device,
	                               QueueVk queue_graphics,
	                               QueueVk queue_compute,
	                               QueueVk queue_transfer) noexcept
		: _instance(instance),
		  _physical_device(physical_device),
		  _logical_device(logical_device),
		  _queue_graphics(queue_graphics),
		  _queue_compute(queue_compute),
		  _queue_transfer(queue_transfer)
	{
	}

	RenderDeviceVk::~RenderDeviceVk() noexcept
	{
		// TODO(SandNoodle): Destroy + Allocation callbacks.
		vkDestroyDevice(_logical_device, nullptr);
		vkDestroyInstance(_instance, nullptr);
	}

	[[nodiscard]] static constexpr VkInstance CreateVulkanInstance() noexcept
	{
		if (!glfwInit()) {
			ORION_LOG_FATAL("[Vulkan] Failed to initialize GLFW.");
			return VK_NULL_HANDLE;
		}

		if (volkInitialize() != VK_SUCCESS) {
			ORION_LOG_FATAL("[Vulkan] Failed to initialize Volk.");
			return VK_NULL_HANDLE;
		}

		VkApplicationInfo application_info{
			.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext              = nullptr,
			.pApplicationName   = "Orion Engine: Editor",
			.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
			.pEngineName        = ORION_ENGINE_NAME,
			.engineVersion      = VK_MAKE_API_VERSION(
                0, ORION_ENGINE_VERSION_MAJOR, ORION_ENGINE_VERSION_MINOR, ORION_ENGINE_VERSION_PATCH),
			.apiVersion = k_vulkan_api_version,
		};

		UInt32 required_extension_count = 0;
		CString* required_extensions    = glfwGetRequiredInstanceExtensions(&required_extension_count);

		static constexpr Array k_validation_layer_names = { "VK_LAYER_KHRONOS_validation" };

		VkInstanceCreateInfo instance_create_info{
			.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext                   = nullptr,
			.flags                   = 0,
			.pApplicationInfo        = &application_info,
			.enabledLayerCount       = k_validation_layer_names.Size(),
			.ppEnabledLayerNames     = k_validation_layer_names.Data(),
			.enabledExtensionCount   = required_extension_count,
			.ppEnabledExtensionNames = required_extensions,
		};
		VkInstance instance = VK_NULL_HANDLE;
		if (vkCreateInstance(&instance_create_info, nullptr, &instance) != VK_SUCCESS) {
			ORION_LOG_FATAL("[Vulkan] Failed to initialize Vulkan (VkInstance).");
			return VK_NULL_HANDLE;
		}
		volkLoadInstance(instance);
		return instance;
	}

	[[nodiscard]] static constexpr VkPhysicalDevice SelectPhysicalDevice(VkInstance instance) noexcept
	{
		ORION_ASSERT_DEBUG(instance != VK_NULL_HANDLE);

		// 1. Acquire the list of physical devices present in the system.
		UInt32 physical_devices_count = 0;
		VkResult result               = vkEnumeratePhysicalDevices(instance, &physical_devices_count, nullptr);
		if (!(result == VK_SUCCESS || result == VK_INCOMPLETE) || physical_devices_count == 0) {
			ORION_LOG_FATAL(
				"[Vulkan] Failed to acquire amount of physical devices present in the system "
				"(vkEnumeratePhysicalDevices != VK_SUCCESS | VK_INCOMPLETE || count == 0).");
			return VK_NULL_HANDLE;
		}

		Vector<VkPhysicalDevice> physical_devices{};
		physical_devices.AddZeroed(physical_devices_count);
		result = vkEnumeratePhysicalDevices(instance, &physical_devices_count, physical_devices.Data());
		if (!(result == VK_SUCCESS || result == VK_INCOMPLETE || physical_devices_count == 0)) {
			ORION_LOG_FATAL(
				"[Vulkan] Failed to acquire physical devices present in the system "
				"(vkEnumeratePhysicalDevices != VK_SUCCESS | VK_INCOMPLETE || count == 0).");
			return VK_NULL_HANDLE;
		}

		// 2. Gather information about each Physical Device.
		struct PhysicalDeviceInfo
		{
			UInt32 original_index;
			VkPhysicalDevice device;
			VkPhysicalDeviceProperties physical_device_properties;
		};
		Vector<PhysicalDeviceInfo> physical_device_infos{};
		physical_device_infos.Reserve(physical_devices_count);
		for (UInt32 index = 0; index < physical_devices_count; ++index) {
			VkPhysicalDeviceProperties physical_device_properties{};
			vkGetPhysicalDeviceProperties(physical_devices[index], &physical_device_properties);
			physical_device_infos.AddConstruct(index, physical_devices[index], physical_device_properties);
		}

		// 3. Score each physical device, with better candidates at the front.
		physical_device_infos.Sort([](const PhysicalDeviceInfo& lhs, const PhysicalDeviceInfo& rhs) {
			// Preserve order of devices of the same type.
			if (lhs.physical_device_properties.deviceType == rhs.physical_device_properties.deviceType) {
				return lhs.original_index < rhs.original_index;
			}

			// Prefer (in order): Discrete GPUs > Integrated GPUs > CPUs.
			return lhs.physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
			    || lhs.physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU;
		});

		// 4. Select the final candidate.
		PhysicalDeviceInfo* best_candidate = &physical_device_infos[0];
		ORION_ASSERT_DEBUG(best_candidate);

		return best_candidate[0].device;
	}

	[[nodiscard]] ORION_FORCE_INLINE static constexpr Bool8 VkHasAllFlags(VkFlags flags,
	                                                                      VkFlags requested_flags) noexcept
	{
		return (flags & requested_flags) == requested_flags;
	}

	static constexpr UInt32 k_invalid_queue_index = NumericLimits<UInt32>::Max();
	namespace
	{
		struct VkQueueData
		{
			Vector<VkDeviceQueueCreateInfo> queue_create_infos{};
			Vector<Float32> queue_priorities{};
			UInt32 queue_index_graphics = k_invalid_queue_index;
			UInt32 queue_index_compute  = k_invalid_queue_index;
			UInt32 queue_index_transfer = k_invalid_queue_index;
		};
	}  // namespace

	[[nodiscard]] static VkQueueData SelectLogicalDeviceQueues(VkPhysicalDevice physical_device)
	{
		UInt32 queue_family_property_count{};
		vkGetPhysicalDeviceQueueFamilyProperties2(physical_device, &queue_family_property_count, nullptr);

		Vector<VkQueueFamilyProperties2> queue_family_properties;
		queue_family_properties.AddZeroed(queue_family_property_count);

		// NOTE: Vulkan's validation layers spew a curious error, that the sType must be
		// VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2 when calling vkGetPhysicalDeviceQueueFamilyProperties2(...).
		for (USize index = 0; index < queue_family_property_count; ++index) {
			queue_family_properties[index].sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
		}

		vkGetPhysicalDeviceQueueFamilyProperties2(
			physical_device, &queue_family_property_count, queue_family_properties.Data());

		VkQueueData queue_data{};

		UInt32 queue_family_properties_size = static_cast<UInt32>(queue_family_properties.Size());
		UInt32 queue_priorities_needed      = 0;
		for (UInt32 family_index = 0; family_index < queue_family_properties_size; ++family_index) {
			const VkFlags queue_flags = queue_family_properties[family_index].queueFamilyProperties.queueFlags;
			UInt32 queue_count        = queue_family_properties[family_index].queueFamilyProperties.queueCount;
			Bool8 is_valid_queue      = false;

			// Prefer Queue Families which support all the requested queue types.
			if (VkHasAllFlags(queue_flags, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT)) {
				queue_data.queue_index_graphics = family_index;
				queue_data.queue_index_compute  = family_index;
				queue_data.queue_index_transfer = family_index;

				Vector<Float32> queue_priorities{};
				queue_priorities.Reserve(queue_count);
				for (USize queue_priority_index = 0; queue_priority_index < queue_count; ++queue_priority_index) {
					queue_priorities.Add(static_cast<Float32>(queue_priority_index));
				}
				queue_data.queue_create_infos.AddConstruct(VkDeviceQueueCreateInfo{
					.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					.pNext            = nullptr,
					.flags            = 0,
					.queueFamilyIndex = family_index,
					.queueCount       = queue_count,
					.pQueuePriorities = queue_priorities.Data(),
				});
				queue_priorities_needed = queue_count;
				break;
			}

			if (VkHasAllFlags(queue_flags, VK_QUEUE_GRAPHICS_BIT)
			    && queue_data.queue_index_graphics == k_invalid_queue_index) {
				queue_data.queue_index_graphics = family_index;
				is_valid_queue                  = true;
			}
			if (VkHasAllFlags(queue_flags, VK_QUEUE_COMPUTE_BIT)
			    && queue_data.queue_index_compute == k_invalid_queue_index) {
				queue_data.queue_index_compute = family_index;
				is_valid_queue                 = true;
			}
			if (VkHasAllFlags(queue_flags, VK_QUEUE_TRANSFER_BIT)
			    && queue_data.queue_index_transfer == k_invalid_queue_index) {
				queue_data.queue_index_transfer = family_index;
				is_valid_queue                  = true;
			}

			// NOTE: Skip queue families, which do not support the requested queue types.
			if (!is_valid_queue) {
				continue;
			}

			queue_data.queue_create_infos.AddConstruct(VkDeviceQueueCreateInfo{
				.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.pNext            = nullptr,
				.flags            = 0,
				.queueFamilyIndex = family_index,
				.queueCount       = queue_count,
				.pQueuePriorities = nullptr,
			});
			queue_priorities_needed += queue_count;
		}

		queue_data.queue_priorities.AddZeroed(queue_priorities_needed);
		Float32* current_queue_priority = queue_data.queue_priorities.Data();
		for (USize index = 0; index < queue_data.queue_create_infos.Size(); ++index) {
			VkDeviceQueueCreateInfo& queue_create_info = queue_data.queue_create_infos[index];
			queue_create_info.pQueuePriorities         = current_queue_priority;

			UInt32 queue_count
				= queue_family_properties[queue_create_info.queueFamilyIndex].queueFamilyProperties.queueCount;
			for (USize queue_index = 0; queue_index < queue_count; ++queue_index) {
				*current_queue_priority++ = 1.0f;
			}
		}

		return queue_data;
	}

	[[nodiscard]] static constexpr VkDevice CreateLogicalDevice(
		VkPhysicalDevice physical_device,
		ReadonlySpan<VkDeviceQueueCreateInfo> queue_create_infos) noexcept
	{
		ORION_ASSERT(physical_device != VK_NULL_HANDLE,
		             "[Vulkan] Failed to create Logical Device, because Physical Device was invalid (VK_NULL_HANDLE).");

		static constexpr Array k_device_extensions_names = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		VkPhysicalDeviceVulkan13Features enabled_13_features{};
		enabled_13_features.sType            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
		enabled_13_features.pNext            = nullptr;
		enabled_13_features.synchronization2 = VK_TRUE;
		enabled_13_features.dynamicRendering = VK_TRUE;

		VkPhysicalDeviceVulkan12Features enabled_12_features{};
		enabled_12_features.sType              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
		enabled_12_features.pNext              = &enabled_13_features;
		enabled_12_features.descriptorIndexing = VK_TRUE;
		enabled_12_features.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
		enabled_12_features.descriptorBindingVariableDescriptorCount  = VK_TRUE;
		enabled_12_features.descriptorBindingVariableDescriptorCount  = VK_TRUE;
		enabled_12_features.runtimeDescriptorArray                    = VK_TRUE;
		enabled_12_features.bufferDeviceAddress                       = VK_TRUE;

		VkPhysicalDeviceFeatures enabled_10_features{};
		enabled_10_features.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo logical_device_create_info{
			.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext                   = &enabled_12_features,
			.flags                   = 0,
			.queueCreateInfoCount    = static_cast<UInt32>(queue_create_infos.Size()),
			.pQueueCreateInfos       = queue_create_infos.Data(),
			.enabledLayerCount       = 0 /* Deprecated */,
			.ppEnabledLayerNames     = nullptr /* Deprecated */,
			.enabledExtensionCount   = k_device_extensions_names.Size(),
			.ppEnabledExtensionNames = k_device_extensions_names.Data(),
			.pEnabledFeatures        = &enabled_10_features,
		};
		VkDevice logical_device = VK_NULL_HANDLE;
		VkResult logical_device_result
			= vkCreateDevice(physical_device, &logical_device_create_info, nullptr, &logical_device);
		if (logical_device_result != VK_SUCCESS) {
			ORION_LOG_FATAL("[Vulkan] Failed to create logical device (vkCreateDevice != VK_SUCCESS).");
			return VK_NULL_HANDLE;
		}

		volkLoadDevice(logical_device);

		return logical_device;
	}

	[[nodiscard]] static constexpr VmaAllocator CreateVmaAllocator(VkInstance instance,
	                                                               VkPhysicalDevice physical_device,
	                                                               VkDevice logical_device) noexcept
	{
		VmaAllocator vma_allocator = VK_NULL_HANDLE;

		VmaVulkanFunctions vma_vulkan_functions{};
		VmaAllocatorCreateInfo vma_allocator_create_info{
			.flags                          = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
			.physicalDevice                 = physical_device,
			.device                         = logical_device,
			.preferredLargeHeapBlockSize    = 0 /* use default */,
			.pAllocationCallbacks           = nullptr,
			.pDeviceMemoryCallbacks         = nullptr,
			.pHeapSizeLimit                 = nullptr,
			.pVulkanFunctions               = &vma_vulkan_functions,
			.instance                       = instance,
			.vulkanApiVersion               = k_vulkan_api_version,
			.pTypeExternalMemoryHandleTypes = nullptr,
		};
		vmaImportVulkanFunctionsFromVolk(&vma_allocator_create_info, &vma_vulkan_functions);
		if (vmaCreateAllocator(&vma_allocator_create_info, &vma_allocator) != VK_SUCCESS) {
			ORION_LOG_FATAL("[Vulkan] Failed to initialize the Vulkan Memory Allocator.");
			return VK_NULL_HANDLE;
		}
		return vma_allocator;
	}

	RenderDeviceVk* RenderDeviceVk::Create()
	{
		VkInstance instance              = CreateVulkanInstance();
		VkPhysicalDevice physical_device = SelectPhysicalDevice(instance);
		VkQueueData queue_data           = SelectLogicalDeviceQueues(physical_device);
		VkDevice logical_device    = CreateLogicalDevice(physical_device, ReadonlySpan(queue_data.queue_create_infos));
		VmaAllocator vma_allocator = CreateVmaAllocator(instance, physical_device, logical_device);

		QueueVk queue_graphics{};
		QueueVk queue_compute{};
		QueueVk queue_transfer{};

		vkGetDeviceQueue(logical_device, queue_data.queue_index_graphics, 0, &queue_graphics.queue);
		vkGetDeviceQueue(logical_device, queue_data.queue_index_compute, 0, &queue_compute.queue);
		vkGetDeviceQueue(logical_device, queue_data.queue_index_transfer, 0, &queue_transfer.queue);

		queue_graphics.queue_family_index = queue_data.queue_index_graphics;
		queue_compute.queue_family_index  = queue_data.queue_index_compute;
		queue_transfer.queue_family_index = queue_data.queue_index_transfer;

		return new RenderDeviceVk(
			instance, physical_device, logical_device, queue_graphics, queue_compute, queue_transfer);
	}

	void RenderDeviceVk::Destroy(RenderDeviceVk* render_device)
	{
		if (!render_device) {
			return;
		}
		delete render_device;
	}
}  // namespace Orion::Engine::Platform

#endif  // ORION_GRAPHICS_VULKAN
