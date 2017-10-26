#include "VulkanCommandBuffer.hpp"
#include "VulkanCommandQueue.hpp"


namespace Ryujin
{
	void VulkanCommandBuffer::Commit()
	{
		VkSubmitInfo info = {};
		// TODO: setup info
		vkQueueSubmit(queue->GetHandle(), 1, &info, VK_NULL_HANDLE);
	}

	VulkanGfxEncoder* VulkanCommandBuffer::NewGfxEncoderFromDescriptor(const RenderPassDescriptor& desc)
	{
		VulkanGfxEncoder* encoder = RYUJIN_NEW VulkanGfxEncoder();
		encoder->cmdBuffer = this;

		return encoder;
	}

	void VulkanCommandBuffer::WaitUntilDone()
	{

	}

	void VulkanCommandBuffer::Release()
	{

	}
}