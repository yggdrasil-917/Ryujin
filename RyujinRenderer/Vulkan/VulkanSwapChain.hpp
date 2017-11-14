#pragma once

#include "../Common/SwapChainBase.hpp"
#include "VulkanBase.hpp"


namespace Ryujin
{
	class RENDER_API VulkanBackBufferImage
	{
	public:
		uint32 imageIndex = 0;
	};

	class RENDER_API VulkanSwapChain : public SwapChainBase
	{
	public:
		struct SwapChainBuffer
		{
			VkImage image;
			VkImageView view;
		};

	private:
		VkSurfaceKHR surface;
		VkSwapchainKHR swapChain;
		VkSurfaceFormatKHR swapChainFormat;
		DynamicArray<SwapChainBuffer> swapChainBuffers;

		bool CreateSurface(VkSurfaceKHR& surface);

	public:
		VIRTUAL ~VulkanSwapChain();

		bool Create(uint16 inWidth, uint16 inHeight, uint32 numImages);
		void Destroy();
		VulkanBackBufferImage* GetNextImage();
	};
}