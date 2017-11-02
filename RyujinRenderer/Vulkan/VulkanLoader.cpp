

#include "VulkanBase.hpp"

#include "RyujinCore/OS/Platform.hpp"

namespace Ryujin
{
	LibModule VulkanLoader::vulkanModule;
	bool VulkanLoader::bHasLoadedVulkan = false;

	PFN_vkCreateInstance vkCreateInstance = nullptr;
	PFN_vkCreateDevice vkCreateDevice = nullptr;
	PFN_vkGetDeviceQueue vkGetDeviceQueue = nullptr;
	PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = nullptr;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = nullptr;
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR = nullptr;
	PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties = nullptr;
	PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = nullptr;
	PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = nullptr;
	PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties = nullptr;
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = nullptr;
	PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = nullptr;
	PFN_vkDestroyDevice vkDestroyDevice = nullptr;
	PFN_vkDestroyInstance vkDestroyInstance = nullptr;
	PFN_vkCreateCommandPool vkCreateCommandPool = nullptr;
	PFN_vkBeginCommandBuffer vkBeginCommandBuffer = nullptr;
	PFN_vkEndCommandBuffer vkEndCommandBuffer = nullptr;
	PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier = nullptr;
	PFN_vkCmdClearColorImage vkCmdClearColorImage = nullptr; 
	PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers = nullptr;
	PFN_vkFreeCommandBuffers vkFreeCommandBuffers = nullptr;
	PFN_vkDestroyCommandPool vkDestroyCommandPool = nullptr;
	PFN_vkQueueSubmit vkQueueSubmit = nullptr;
	PFN_vkQueueWaitIdle vkQueueWaitIdle = nullptr;
	PFN_vkCreateImage vkCreateImage = nullptr;
	PFN_vkCreateImageView vkCreateImageView = nullptr;
	PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements = nullptr;
	PFN_vkBindImageMemory vkBindImageMemory = nullptr;
	PFN_vkDestroyImageView vkDestroyImageView = nullptr;
	PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties = nullptr;
	PFN_vkCreateSampler vkCreateSampler = nullptr;
	PFN_vkDestroySampler vkDestroySampler = nullptr;

	PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR = nullptr;
	PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR = nullptr;
	PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR = nullptr;
	PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR = nullptr;
	PFN_vkQueuePresentKHR vkQueuePresentKHR = nullptr;
	PFN_vkCreateFence vkCreateFence = nullptr;
	PFN_vkWaitForFences vkWaitForFences = nullptr;
	PFN_vkResetFences vkResetFences = nullptr;
	PFN_vkResetCommandBuffer vkResetCommandBuffer = nullptr;
	PFN_vkCreateSemaphore vkCreateSemaphore = nullptr;
	PFN_vkDestroySemaphore vkDestroySemaphore = nullptr;
	PFN_vkDeviceWaitIdle vkDeviceWaitIdle = nullptr;

	PFN_vkCreateFramebuffer vkCreateFramebuffer = nullptr;
	PFN_vkCreateRenderPass vkCreateRenderPass = nullptr;
	PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass = nullptr;
	PFN_vkCmdSetViewport vkCmdSetViewport = nullptr;
	PFN_vkCmdSetScissor vkCmdSetScissor = nullptr;
	PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets = nullptr;
	PFN_vkCmdBindPipeline vkCmdBindPipeline = nullptr;
	PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers = nullptr;
	PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer = nullptr;
	PFN_vkCmdDrawIndexed vkCmdDrawIndexed = nullptr;
	PFN_vkCmdEndRenderPass vkCmdEndRenderPass = nullptr;

	PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR = nullptr;

	PFN_vkCreateBuffer vkCreateBuffer = nullptr;
	PFN_vkDestroyBuffer vkDestroyBuffer = nullptr;
	PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements = nullptr;
	PFN_vkAllocateMemory vkAllocateMemory = nullptr;
	PFN_vkMapMemory vkMapMemory = nullptr;
	PFN_vkUnmapMemory vkUnmapMemory = nullptr;
	PFN_vkBindBufferMemory vkBindBufferMemory = nullptr;
	PFN_vkFreeMemory vkFreeMemory = nullptr;
	PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT = nullptr;
	PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT = nullptr;

	PFN_vkCmdDraw vkCmdDraw = nullptr;

	// Windows functions
#if PLATFORM_WINDOWS
	PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = nullptr;
#elif PLATFORM_LINUX
    PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR = nullptr;
#endif

	// Extensions
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = nullptr;
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = nullptr;
	PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT = nullptr;

	void VulkanLoader::LoadVulkan()
	{
		if (bHasLoadedVulkan)
			return;

        vulkanModule = LoadVulkanLib(VulkanLibrary);
		RYUJIN_ASSERT(vulkanModule, "Failed to load vulkan library");

		LoadVulkanFunction(vulkanModule, vkCreateInstance);
		LoadVulkanFunction(vulkanModule, vkCreateDevice);
		LoadVulkanFunction(vulkanModule, vkGetDeviceQueue);
		LoadVulkanFunction(vulkanModule, vkGetPhysicalDeviceProperties);
		LoadVulkanFunction(vulkanModule, vkGetPhysicalDeviceQueueFamilyProperties);
		LoadVulkanFunction(vulkanModule, vkGetPhysicalDeviceSurfaceSupportKHR);
		LoadVulkanFunction(vulkanModule, vkGetPhysicalDeviceMemoryProperties);
		LoadVulkanFunction(vulkanModule, vkEnumerateInstanceExtensionProperties);
		LoadVulkanFunction(vulkanModule, vkEnumerateInstanceLayerProperties);
		LoadVulkanFunction(vulkanModule, vkEnumerateDeviceExtensionProperties);
		LoadVulkanFunction(vulkanModule, vkGetInstanceProcAddr);
		LoadVulkanFunction(vulkanModule, vkEnumeratePhysicalDevices);
		LoadVulkanFunction(vulkanModule, vkDestroyDevice);
		LoadVulkanFunction(vulkanModule, vkDestroyInstance);
		LoadVulkanFunction(vulkanModule, vkCreateCommandPool);
		LoadVulkanFunction(vulkanModule, vkQueueSubmit);
		LoadVulkanFunction(vulkanModule, vkQueueWaitIdle);
		LoadVulkanFunction(vulkanModule, vkAllocateCommandBuffers);
		LoadVulkanFunction(vulkanModule, vkBeginCommandBuffer);
		LoadVulkanFunction(vulkanModule, vkEndCommandBuffer);
		LoadVulkanFunction(vulkanModule, vkCmdPipelineBarrier);
		LoadVulkanFunction(vulkanModule, vkCmdClearColorImage);
		LoadVulkanFunction(vulkanModule, vkFreeCommandBuffers);
		LoadVulkanFunction(vulkanModule, vkDestroyCommandPool);
		LoadVulkanFunction(vulkanModule, vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
		LoadVulkanFunction(vulkanModule, vkCreateImage);
		LoadVulkanFunction(vulkanModule, vkCreateImageView);
		LoadVulkanFunction(vulkanModule, vkBindImageMemory);
		LoadVulkanFunction(vulkanModule, vkGetImageMemoryRequirements);
		LoadVulkanFunction(vulkanModule, vkDestroyImageView);
		LoadVulkanFunction(vulkanModule, vkGetPhysicalDeviceFormatProperties);
		LoadVulkanFunction(vulkanModule, vkCreateSampler);
		LoadVulkanFunction(vulkanModule, vkDestroySampler);

		LoadVulkanFunction(vulkanModule, vkCreateSwapchainKHR);
		LoadVulkanFunction(vulkanModule, vkDestroySwapchainKHR);
		LoadVulkanFunction(vulkanModule, vkGetSwapchainImagesKHR);
		LoadVulkanFunction(vulkanModule, vkAcquireNextImageKHR);
		LoadVulkanFunction(vulkanModule, vkQueuePresentKHR);
		LoadVulkanFunction(vulkanModule, vkCreateFence);
		LoadVulkanFunction(vulkanModule, vkWaitForFences);
		LoadVulkanFunction(vulkanModule, vkResetFences);
		LoadVulkanFunction(vulkanModule, vkResetCommandBuffer);
		LoadVulkanFunction(vulkanModule, vkCreateSemaphore);
		LoadVulkanFunction(vulkanModule, vkDestroySemaphore);
		LoadVulkanFunction(vulkanModule, vkDeviceWaitIdle);

		LoadVulkanFunction(vulkanModule, vkCreateFramebuffer);
		LoadVulkanFunction(vulkanModule, vkCreateRenderPass);
		LoadVulkanFunction(vulkanModule, vkCmdBeginRenderPass);
		LoadVulkanFunction(vulkanModule, vkCmdSetViewport);
		LoadVulkanFunction(vulkanModule, vkCmdSetScissor);
		LoadVulkanFunction(vulkanModule, vkCmdBindDescriptorSets);
		LoadVulkanFunction(vulkanModule, vkCmdBindPipeline);
		LoadVulkanFunction(vulkanModule, vkCmdBindVertexBuffers);
		LoadVulkanFunction(vulkanModule, vkCmdBindIndexBuffer);
		LoadVulkanFunction(vulkanModule, vkCmdDrawIndexed);
		LoadVulkanFunction(vulkanModule, vkCmdEndRenderPass);

		LoadVulkanFunction(vulkanModule, vkDestroySurfaceKHR);
		LoadVulkanFunction(vulkanModule, vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
		LoadVulkanFunction(vulkanModule, vkGetPhysicalDeviceSurfaceFormatsKHR);
		LoadVulkanFunction(vulkanModule, vkGetPhysicalDeviceSurfacePresentModesKHR);

		LoadVulkanFunction(vulkanModule, vkCreateBuffer);
		LoadVulkanFunction(vulkanModule, vkDestroyBuffer);
		LoadVulkanFunction(vulkanModule, vkGetBufferMemoryRequirements);
		LoadVulkanFunction(vulkanModule, vkAllocateMemory);
		LoadVulkanFunction(vulkanModule, vkMapMemory);
		LoadVulkanFunction(vulkanModule, vkUnmapMemory);
		LoadVulkanFunction(vulkanModule, vkBindBufferMemory);
		LoadVulkanFunction(vulkanModule, vkFreeMemory);

		LoadVulkanFunction(vulkanModule, vkCmdDebugMarkerInsertEXT);
		LoadVulkanFunction(vulkanModule, vkCmdDebugMarkerEndEXT);

		LoadVulkanFunction(vulkanModule, vkCmdDraw);

#if PLATFORM_WINDOWS
		LoadVulkanFunction(vulkanModule, vkCreateWin32SurfaceKHR);
#elif PLATFORM_LINUX
        LoadVulkanFunction(vulkanModule, vkCreateXcbSurfaceKHR);
#endif

		bHasLoadedVulkan = true;
	}

	void VulkanLoader::LoadVulkanExtensions(VkInstance& instance)
	{
		LoadVulkanFunctionExt(instance, vkCreateDebugReportCallbackEXT);
		LoadVulkanFunctionExt(instance, vkDestroyDebugReportCallbackEXT);
		LoadVulkanFunctionExt(instance, vkDebugReportMessageEXT);
	}

	void VulkanLoader::UnloadVulkan()
	{
#if PLATFORM_WINDOWS
		FreeLibrary(vulkanModule);
#endif
		bHasLoadedVulkan = false;
	}
}
