#pragma once


#include "../Renderer/RendererBase.hpp"


namespace Ryujin
{
	class VulkanBuffer
	{

	};

	class ConstantBuffer
	{
	public:
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDescriptorBufferInfo descriptor;
		uint32 allocSize;
		void* mapped = nullptr;
	};
}