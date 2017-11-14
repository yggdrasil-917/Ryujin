#include "VulkanSwapChain.hpp"

#include "VulkanRendererCommon.hpp"

namespace Ryujin
{
	VulkanSwapChain::~VulkanSwapChain()
	{
		Destroy();
	}

	bool VulkanSwapChain::Create(uint16 inWidth, uint16 inHeight, uint32 numImages)
	{
		VkDevice device = VulkanDevice::Instance()->GetDevice();
		VkPhysicalDevice physicalDevice = VulkanDevice::Instance()->GetPhysicalDevice();

		if (swapChain != VK_NULL_HANDLE || width != inWidth || height != inHeight)
		{
			Destroy();
		}

		width = inWidth;
		height = inHeight;

		// Check for swap chain support on running hardware
		VkSurfaceKHR surface = VulkanDevice::Instance()->GetSurface();
		VkSurfaceCapabilitiesKHR surfaceCaps;
		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCaps) != VK_SUCCESS)
		{
			return false;
		}

		// Query surface formats
		uint32 surfaceFormatCount = 0;
		if ((vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr) != VK_SUCCESS) || surfaceFormatCount == 0)
		{
			return false;
		}

		VkSurfaceFormatKHR* formats = RYUJIN_NEW VkSurfaceFormatKHR[surfaceFormatCount];
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, formats) != VK_SUCCESS)
		{
			return false;
		}

		// Get present modes
		uint32 presentModeCount = 0;
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr) != VK_SUCCESS || presentModeCount == 0)
		{
			return false;
		}

		VkPresentModeKHR* presentModes = RYUJIN_NEW VkPresentModeKHR[presentModeCount];
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes) != VK_SUCCESS)
		{
			return false;
		}
		VkPresentModeKHR selectedPresentMode = VK_PRESENT_MODE_FIFO_KHR;
		for (uint32 i = 0; i < presentModeCount; ++i)
		{
			// Prefer mailbox mode if it is available as it is the lowest latency non-tearing mode, otherwise
			// immediate is the fastest but doest tear; if not then fallback to fifo which should always be available
			// https://software.intel.com/en-us/articles/api-without-secrets-introduction-to-vulkan-part-2
			if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				selectedPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
				break;
			}
			if (selectedPresentMode != VK_PRESENT_MODE_MAILBOX_KHR && presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				selectedPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
		}

		// minImageCount contains the min number of images for swap chain to work correctly, I add 1 to it and check that it is still safe to do so as 
		// triple buffering can be useful to have
		uint32 imageCount = surfaceCaps.minImageCount + 1;
		if (surfaceCaps.maxImageCount > 0 && imageCount > surfaceCaps.maxImageCount)
		{
			imageCount = surfaceCaps.maxImageCount;
		}

		VkSurfaceFormatKHR selectedSwapChainFormat = { VK_FORMAT_UNDEFINED , VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		// If the list contains only one entry with undefined format
		// it means that there are no preferred surface formats and any can be chosen
		if ((surfaceFormatCount == 1) && (formats[0].format == VK_FORMAT_UNDEFINED))
		{
			selectedSwapChainFormat = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		}

		for (uint32 i = 0; i < surfaceFormatCount; ++i)
		{
			if (formats[i].format == VK_FORMAT_R8G8B8A8_UNORM)
			{
				selectedSwapChainFormat = formats[i];
				break;
			}
		}

		if (selectedSwapChainFormat.format == VK_FORMAT_UNDEFINED)
		{
			selectedSwapChainFormat = formats[0];
		}
		swapChainFormat = selectedSwapChainFormat;

		// VK_IMAGE_USAGE_TRANSFER_DST_BIT is required for image clear operation.
		VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		VkExtent2D swapChainExtent = { width, height };
		if (surfaceCaps.currentExtent.width == -1)
		{
			if (swapChainExtent.width < surfaceCaps.minImageExtent.width)
			{
				swapChainExtent.width = surfaceCaps.minImageExtent.width;
			}
			if (swapChainExtent.width > surfaceCaps.maxImageExtent.width)
			{
				swapChainExtent.width = surfaceCaps.maxImageExtent.width;
			}
			if (swapChainExtent.height < surfaceCaps.minImageExtent.height)
			{
				swapChainExtent.height = surfaceCaps.minImageExtent.height;
			}
			if (swapChainExtent.height > surfaceCaps.maxImageExtent.height)
			{
				swapChainExtent.height = surfaceCaps.maxImageExtent.height;
			}
		}

		VkSwapchainKHR oldSwapChain = swapChain;
		VkSwapchainCreateInfoKHR swapChainCreateInfo =
		{
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			nullptr,
			0,
			surface,
			imageCount,
			selectedSwapChainFormat.format,
			selectedSwapChainFormat.colorSpace,
			swapChainExtent,
			1,
			usageFlags,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			nullptr,
			VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			selectedPresentMode,
			VK_TRUE,
			oldSwapChain
		};

		if (vkCreateSwapchainKHR(device, &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS)
		{
			return false;
		}

		if (oldSwapChain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(device, oldSwapChain, nullptr);
		}

		uint32 swapChainCount = 0;
		VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device, swapChain, &swapChainCount, nullptr));

		VkImage* swapChainImages = RYUJIN_NEW VkImage[swapChainCount];
		if (vkGetSwapchainImagesKHR(device, swapChain, &swapChainCount, swapChainImages) != VK_SUCCESS)
		{
			return false;
		}

		swapChainBuffers.Resize(swapChainCount);
		for (uint32 i = 0; i < swapChainCount; i++)
		{
			VkImageViewCreateInfo colorAttachmentView = {};
			colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			colorAttachmentView.pNext = NULL;
			colorAttachmentView.format = swapChainFormat.format;
			colorAttachmentView.components = {
				VK_COMPONENT_SWIZZLE_R,
				VK_COMPONENT_SWIZZLE_G,
				VK_COMPONENT_SWIZZLE_B,
				VK_COMPONENT_SWIZZLE_A
			};
			colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			colorAttachmentView.subresourceRange.baseMipLevel = 0;
			colorAttachmentView.subresourceRange.levelCount = 1;
			colorAttachmentView.subresourceRange.baseArrayLayer = 0;
			colorAttachmentView.subresourceRange.layerCount = 1;
			colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
			colorAttachmentView.flags = 0;

			swapChainBuffers[i].image = swapChainImages[i];

			//VkUtils::SetImageLayout(setupCmdBuffer, swapChainBuffers[i].image, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

			colorAttachmentView.image = swapChainBuffers[i].image;

			if (vkCreateImageView(device, &colorAttachmentView, nullptr, &swapChainBuffers[i].view) != VK_SUCCESS)
			{
				return false;
			}
		}

		SAFE_DELETE_ARRAY(swapChainImages);
		SAFE_DELETE_ARRAY(formats);
		SAFE_DELETE_ARRAY(presentModes);

		return true;
	}

	VulkanBackBufferImage* VulkanSwapChain::GetNextImage()
	{
		VulkanBackBufferImage* bb = RYUJIN_NEW VulkanBackBufferImage();

		uint32 imageIndex;
		VK_CHECK_RESULT(vkAcquireNextImageKHR(VulkanDevice::Instance()->GetDevice(), swapChain, NumericLimit<uint64>::Max(), 
			VulkanDevice::Instance()->GetPresentCompleteSem(), VK_NULL_HANDLE, &imageIndex));

		bb->imageIndex = imageIndex;
		return bb;
	}

	void VulkanSwapChain::Destroy()
	{
		VkDevice device = VulkanDevice::Instance()->GetDevice();
		for (uint32_t i = 0; i < swapChainBuffers.GetSize(); ++i)
		{
			vkDestroyImageView(device, swapChainBuffers[i].view, nullptr);
		}
		vkDestroySwapchainKHR(device, swapChain, nullptr);
		swapChainBuffers.RemoveAll();
		swapChain = VK_NULL_HANDLE;
	}
}