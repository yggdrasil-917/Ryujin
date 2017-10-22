#pragma once

#include "../Renderer/RendererBase.hpp"
#include <cassert>

#if PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR 1

#elif PLATFORM_LINUX
#include <dlfcn.h>
#define VK_USE_PLATFORM_XCB_KHR 1

#else
#error "Vulkan platform not specified"
#endif

#define VK_NO_PROTOTYPES

#include <vulkan/vulkan.h>

#define ENABLE_VALIDATION (_DEBUG || FORCE_ENABLE_VULKAN_VALIDATION)

#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		Errorf(Renderer, "Fatal : VkResult is \"%s\"", STRINGIZE(res));									\
		assert(res == VK_SUCCESS);																		\
	}																									\
}	

#define VK_VER_MAJOR(X) ((((uint32_t)(X))>>22)&0x3FF)
#define VK_VER_MINOR(X) ((((uint32_t)(X))>>12)&0x3FF)
#define VK_VER_PATCH(X) (((uint32_t)(X)) & 0xFFF)

namespace Ryujin
{
#include "VulkanFunctionList.inl"

#if PLATFORM_WINDOWS
#define VulkanGetProcAddress GetProcAddress
#define VulkanLibrary "vulkan-1.dll"
#define LoadDynamicLib LoadLibrary
#define LoadVulkanLib(x) LoadLibrary(x)

#elif PLATFORM_LINUX
#define VulkanGetProcAddress dlsym
#define LoadDynamicLib dlopen
#define VulkanLibrary "libvulkan.so"
#define LoadVulkanLib(x) dlopen(x, RTLD_LAZY)
#endif


#define LoadVulkanFunction(module, function) \
	function = (PFN_##function)VulkanGetProcAddress(module, #function); \
	RYUJIN_ASSERT(function, CONCAT_STR_WITH_SPACE(Failed to load vulkan function, function));

#define LoadVulkanFunctionExt(instance, function) \
    function = (PFN_##function)vkGetInstanceProcAddr(instance, #function)
//*(void**)&function = vkGetInstanceProcAddr(instance, #function)

	class RENDER_API VulkanLoader
	{
	private:
		static LibModule vulkanModule;
		static bool bHasLoadedVulkan;

	public:
		static void LoadVulkan();
		static void LoadVulkanExtensions(VkInstance& instance);
		static void UnloadVulkan();
	};

	namespace VkUtils
	{
		void SetImageLayout(
			VkCommandBuffer cmdbuffer,
			VkImage image,
			VkImageAspectFlags aspectMask,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout,
			VkImageSubresourceRange subresourceRange);

		void SetImageLayout(
			VkCommandBuffer cmdbuffer,
			VkImage image,
			VkImageAspectFlags aspectMask,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout);

		namespace Initializers
		{
			VkCommandBufferAllocateInfo CommandBufferAllocateInfo(VkCommandPool commandPool, VkCommandBufferLevel level, uint32 bufferCount);
			VkSemaphoreCreateInfo SemaphoreCreateInfo();
			VkImageMemoryBarrier ImageMemoryBarrier();
			VkBufferMemoryBarrier BufferMemoryBarrier();
			VkMemoryBarrier MemoryBarrier();

			VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlags usageFlags, const VkCommandBufferInheritanceInfo* inheritanceInfo = nullptr);
		}
	}
}
