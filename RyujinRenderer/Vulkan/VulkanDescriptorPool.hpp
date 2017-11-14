#pragma once

#include "VulkanBase.hpp"


namespace Ryujin
{
	struct DescriptorPoolSizeDesc
	{
		VkDescriptorType type;
		uint32 count;
	};

	class RENDER_API VulkanDescriptorPool
	{
	private:
		VkDescriptorPool handle;

	public:
		~VulkanDescriptorPool();
		bool Create(const DescriptorPoolSizeDesc* desc, uint32 descCount, uint32 maxSetsToAllocate);
	};
}