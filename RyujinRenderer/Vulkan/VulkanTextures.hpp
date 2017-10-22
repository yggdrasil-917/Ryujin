#pragma once


#include "../Renderer/RendererBase.hpp"


namespace Ryujin
{
	class VulkanTexture
	{
	private:
		VkSampler sampler;

		VkImage image;
		VkImageLayout imageLayout;

		VkDeviceMemory mem;
		VkImageView view;
		int32 width;
		int32 height;
	};

	class VulkanSampler
	{

	};
}