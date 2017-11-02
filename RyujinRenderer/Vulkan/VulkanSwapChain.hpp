#pragma once

#include "../Common/SwapChainBase.hpp"
#include "VulkanBase.hpp"


namespace Ryujin
{
	class RENDER_API VulkanBackBufferImage
	{
	private:

	public:

	};

	class RENDER_API VulkanSwapChain : public SwapChainBase
	{
	public:
		VIRTUAL ~VulkanSwapChain() {}

		void Create(uint16 inWidth, uint16 inHeight, uint32 numImages);
		VulkanBackBufferImage* GetNextImage();
	};
}