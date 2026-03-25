#if defined(ORION_GRAPHICS_VULKAN)
#include "Platform/Graphics/Vulkan/RenderDeviceVk.h"

#include "Core/Log.h"
#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Containers/Span.h"
#include "Core/Standard/Containers/Vector.h"
#include "Core/Standard/Limits.h"

#define VOLK_IMPLEMENTATION
#include "volk.h"

#define VMA_IMPLEMENTATION
#include "vma/vk_mem_alloc.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Orion::Engine::Platform
{
	static constexpr UInt32 k_vulkan_api_version = VK_API_VERSION_1_3;

	RenderDeviceVk::RenderDeviceVk(VkInstance instance) : _instance(instance) {}

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

		// -- Instance
		UInt32 required_extension_count = 0;
		auto* required_extensions       = glfwGetRequiredInstanceExtensions(&required_extension_count);
		VkApplicationInfo application_info{
			.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext            = nullptr,
			.pApplicationName = "Orion Engine: Editor",
			.pEngineName      = ORION_ENGINE_NAME,
			.engineVersion
			= VK_MAKE_VERSION(ORION_ENGINE_VERSION_MAJOR, ORION_ENGINE_VERSION_MINOR, ORION_ENGINE_VERSION_PATCH),
			.apiVersion = k_vulkan_api_version,
		};
		VkInstanceCreateInfo instance_create_info{
			.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext                   = nullptr,
			.flags                   = 0,
			.pApplicationInfo        = &application_info,
			.enabledLayerCount       = 0,
			.ppEnabledLayerNames     = nullptr,
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
		VkResult result               = VK_SUCCESS;
		UInt32 physical_devices_count = 0;
		result                        = vkEnumeratePhysicalDevices(instance, &physical_devices_count, nullptr);
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

	template <typename BitFlag>
	[[nodiscard]] ORION_FORCE_INLINE static constexpr Bool8 VkHasAllFlags(VkFlags flags,
	                                                                      BitFlag requested_flags) noexcept
	{
		return (flags & requested_flags) == requested_flags;
	}

	[[nodiscard]] static Vector<VkDeviceQueueCreateInfo> SelectLogicalDeviceQueues(VkPhysicalDevice physical_device)
	{
		UInt32 queue_family_property_count{};
		vkGetPhysicalDeviceQueueFamilyProperties2(physical_device, &queue_family_property_count, nullptr);

		Vector<VkQueueFamilyProperties2> queue_family_properties;
		queue_family_properties.AddZeroed(queue_family_property_count);
		vkGetPhysicalDeviceQueueFamilyProperties2(
			physical_device, &queue_family_property_count, queue_family_properties.Data());

		static constexpr UInt32 k_invalid_queue_index = NumericLimits<UInt32>::Max();
		UInt32 queue_index_graphics                   = k_invalid_queue_index;
		UInt32 queue_index_compute                    = k_invalid_queue_index;
		UInt32 queue_index_transfer                   = k_invalid_queue_index;

		Vector<VkDeviceQueueCreateInfo> queue_create_infos{};
		for (USize family_index = 0; family_index < queue_family_properties.Size(); ++family_index) {
			const VkFlags queue_flags = queue_family_properties[family_index].queueFamilyProperties.queueFlags;
			Bool8 is_valid_queue      = false;

			// Prefer Queue Families which support all the requested queues.
			if (VkHasAllFlags(queue_flags, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT)) {
				queue_index_graphics = family_index;
				queue_index_compute  = family_index;
				queue_index_transfer = family_index;

				UInt32 queue_count = queue_family_properties[family_index].queueFamilyProperties.queueCount;
				Vector<Float32> queue_priorities{};
				queue_priorities.Reserve(queue_count);
				for (USize queue_priority_index = 0; queue_priority_index < queue_count; ++queue_priority_index) {
					queue_priorities.Add(static_cast<Float32>(queue_priority_index));
				}
				queue_create_infos.AddConstruct(VkDeviceQueueCreateInfo{
					.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					.pNext            = nullptr,
					.flags            = 0,
					.queueFamilyIndex = static_cast<UInt32>(family_index),
					.queueCount       = queue_count,
					.pQueuePriorities = queue_priorities.Data(),
				});
				break;
			}

			if (VkHasAllFlags(queue_flags, VK_QUEUE_GRAPHICS_BIT) && queue_index_graphics == k_invalid_queue_index) {
				queue_index_graphics = family_index;
				is_valid_queue       = true;
			}
			if (VkHasAllFlags(queue_flags, VK_QUEUE_COMPUTE_BIT) && queue_index_compute == k_invalid_queue_index) {
				queue_index_compute = family_index;
				is_valid_queue      = true;
			}
			if (VkHasAllFlags(queue_flags, VK_QUEUE_TRANSFER_BIT) && queue_index_transfer == k_invalid_queue_index) {
				queue_index_transfer = family_index;
				is_valid_queue       = true;
			}

			// NOTE: Skip queue families, which do not support the requested queues.
			if (!is_valid_queue) {
				continue;
			}

			// TODO(SandNoodle): Add queue family to queue_create_infos.
		}

		for (USize index = 0; index < queue_create_infos.Size(); ++index) {
		}

		return queue_create_infos;
	}

	[[nodiscard]] static constexpr VkDevice CreateLogicalDevice(
		VkPhysicalDevice physical_device,
		ReadonlySpan<VkDeviceQueueCreateInfo> queue_create_infos) noexcept
	{
		ORION_ASSERT(physical_device != VK_NULL_HANDLE,
		             "[Vulkan] Failed to create Logical Device, because Physical Device was invalid (VK_NULL_HANDLE).");

		static constexpr Array k_validation_layer_names  = { "VK_LAYER_KHRONOS_validation" };
		static constexpr Array k_device_extensions_names = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		VkPhysicalDeviceVulkan13Features enabled_13_features{
			.sType            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
			.pNext            = nullptr,
			.synchronization2 = true,
			.dynamicRendering = true,
		};
		VkPhysicalDeviceVulkan12Features enabled_12_features{
			.sType                                     = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
			.pNext                                     = &enabled_13_features,
			.descriptorIndexing                        = true,
			.shaderSampledImageArrayNonUniformIndexing = true,
			.descriptorBindingVariableDescriptorCount  = true,
			.runtimeDescriptorArray                    = true,
			.bufferDeviceAddress                       = true,
		};

		VkPhysicalDeviceFeatures enabled_10_features{
			.samplerAnisotropy = VK_TRUE,
		};

		VkDeviceCreateInfo logical_device_create_info{
			.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext                   = &enabled_12_features,
			.flags                   = 0,
			.queueCreateInfoCount    = static_cast<UInt32>(queue_create_infos.Size()),
			.pQueueCreateInfos       = queue_create_infos.Data(),
			.enabledLayerCount       = k_validation_layer_names.Size(),
			.ppEnabledLayerNames     = k_validation_layer_names.Data(),
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

	RenderDeviceVk* RenderDeviceVk::Create()
	{
		VkInstance instance              = CreateVulkanInstance();
		VkPhysicalDevice physical_device = SelectPhysicalDevice(instance);
		Vector queue_indices             = SelectLogicalDeviceQueues(physical_device);
		VkDevice logical_device          = CreateLogicalDevice(physical_device, ReadonlySpan(queue_indices));

		// -- Queues
		UInt32 queue_family_property_count{};
		vkGetPhysicalDeviceQueueFamilyProperties2(physical_device, &queue_family_property_count, nullptr);

		Vector<VkQueueFamilyProperties2> queue_family_properties;
		queue_family_properties.AddZeroed(queue_family_property_count);
		vkGetPhysicalDeviceQueueFamilyProperties2(
			physical_device, &queue_family_property_count, queue_family_properties.Data());

		const auto get_queue_index
			= [&queue_family_properties, queue_family_property_count](VkQueueFlagBits flag) -> UInt32 {
			for (USize index = 0; index < queue_family_property_count; ++index) {
				if (queue_family_properties[index].queueFamilyProperties.queueFlags & flag) {
					return index;
				}
			}
			return 0;
		};

		UInt32 graphics_queue_index = get_queue_index(VK_QUEUE_GRAPHICS_BIT);
		UInt32 transfer_queue_index = get_queue_index(VK_QUEUE_TRANSFER_BIT);
		UInt32 compute_queue_index  = get_queue_index(VK_QUEUE_COMPUTE_BIT);

		Float32 queue_priority = 1.0f;
		VkDeviceQueueCreateInfo graphics_queue_create_info{
			.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.pNext            = nullptr,
			.flags            = 0,
			.queueFamilyIndex = graphics_queue_index,
			.queueCount       = 1,
			.pQueuePriorities = &queue_priority,
		};
		VkDeviceQueueCreateInfo transfer_queue_create_info{
			.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.pNext            = nullptr,
			.flags            = 0,
			.queueFamilyIndex = transfer_queue_index,
			.queueCount       = 1,
			.pQueuePriorities = &queue_priority,
		};
		VkDeviceQueueCreateInfo compute_queue_create_info{
			.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.pNext            = nullptr,
			.flags            = 0,
			.queueFamilyIndex = compute_queue_index,
			.queueCount       = 1,
			.pQueuePriorities = &queue_priority,
		};
		Array queue_create_infos
			= { graphics_queue_create_info, transfer_queue_create_info, compute_queue_create_info };

		// -- VMA
		VmaVulkanFunctions vma_vulkan_functions{
			.vkGetInstanceProcAddr                   = vkGetInstanceProcAddr,
			.vkGetDeviceProcAddr                     = vkGetDeviceProcAddr,
			.vkGetPhysicalDeviceProperties           = vkGetPhysicalDeviceProperties,
			.vkGetPhysicalDeviceMemoryProperties     = vkGetPhysicalDeviceMemoryProperties,
			.vkAllocateMemory                        = vkAllocateMemory,
			.vkFreeMemory                            = vkFreeMemory,
			.vkMapMemory                             = vkMapMemory,
			.vkUnmapMemory                           = vkUnmapMemory,
			.vkFlushMappedMemoryRanges               = vkFlushMappedMemoryRanges,
			.vkInvalidateMappedMemoryRanges          = vkInvalidateMappedMemoryRanges,
			.vkBindBufferMemory                      = vkBindBufferMemory,
			.vkBindImageMemory                       = vkBindImageMemory,
			.vkGetBufferMemoryRequirements           = vkGetBufferMemoryRequirements,
			.vkGetImageMemoryRequirements            = vkGetImageMemoryRequirements,
			.vkCreateBuffer                          = vkCreateBuffer,
			.vkDestroyBuffer                         = vkDestroyBuffer,
			.vkCreateImage                           = vkCreateImage,
			.vkDestroyImage                          = vkDestroyImage,
			.vkCmdCopyBuffer                         = vkCmdCopyBuffer,
			.vkGetBufferMemoryRequirements2KHR       = vkGetBufferMemoryRequirements2KHR,
			.vkGetImageMemoryRequirements2KHR        = vkGetImageMemoryRequirements2KHR,
			.vkBindBufferMemory2KHR                  = vkBindBufferMemory2KHR,
			.vkBindImageMemory2KHR                   = vkBindImageMemory2KHR,
			.vkGetPhysicalDeviceMemoryProperties2KHR = vkGetPhysicalDeviceMemoryProperties2KHR,
			.vkGetDeviceBufferMemoryRequirements     = vkGetDeviceBufferMemoryRequirements,
			.vkGetDeviceImageMemoryRequirements      = vkGetDeviceImageMemoryRequirements,
		};

		VmaAllocator vma_allocator = VK_NULL_HANDLE;
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
		VkResult vma_allocator_result = vmaCreateAllocator(&vma_allocator_create_info, &vma_allocator);
		if (vma_allocator_result != VK_SUCCESS) {
			ORION_LOG_FATAL("[Vulkan] Failed to initialize the Vulkan Memory Allocator.");
			return nullptr;
		}

		return new RenderDeviceVk(instance);
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
