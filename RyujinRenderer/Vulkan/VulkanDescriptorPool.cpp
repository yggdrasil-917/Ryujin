#include "VulkanDescriptorPool.hpp"

#include "VulkanRendererCommon.hpp"

namespace Ryujin
{
	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		vkDestroyDescriptorPool(VulkanDevice::Instance()->GetDevice(), handle, nullptr);
	}

	bool VulkanDescriptorPool::Create(const DescriptorPoolSizeDesc* desc, uint32 descCount, uint32 maxSetsToAllocate)
	{
		VkDescriptorPoolSize* poolSizeArray = RYUJIN_NEW VkDescriptorPoolSize[descCount];
		for (uint32 i = 0; i < descCount; ++i)
		{
			poolSizeArray[i] = {};
			poolSizeArray[i].type = desc[i].type;
			poolSizeArray[i].descriptorCount = desc[i].count;
		}

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = descCount;
		poolInfo.pPoolSizes = poolSizeArray;
		poolInfo.maxSets = maxSetsToAllocate;

		if (vkCreateDescriptorPool(VulkanDevice::Instance()->GetDevice(), &poolInfo, nullptr, &handle) != VK_SUCCESS)
		{
			Errorf(Renderer, "failed to create descriptor pool!");
			delete[] poolSizeArray;
			return false;
		}

		delete[] poolSizeArray;
		return true;
	}
}