#include "VulkanBase.hpp"



namespace Ryujin
{
	namespace VkUtils
	{
		namespace Initializers
		{
			VkCommandBufferAllocateInfo CommandBufferAllocateInfo(VkCommandPool commandPool, VkCommandBufferLevel level, uint32 bufferCount)
			{
				VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
				commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				commandBufferAllocateInfo.commandPool = commandPool;
				commandBufferAllocateInfo.level = level;
				commandBufferAllocateInfo.commandBufferCount = bufferCount;
				return commandBufferAllocateInfo;
			}

			VkSemaphoreCreateInfo SemaphoreCreateInfo()
			{
				VkSemaphoreCreateInfo sem = {};
				sem.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
				sem.pNext = nullptr;
				sem.flags = 0;
				return sem;
			}

			VkImageMemoryBarrier ImageMemoryBarrier()
			{
				VkImageMemoryBarrier imageMemoryBarrier = {};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.pNext = NULL;
				// Some default values
				imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				return imageMemoryBarrier;
			}

			VkBufferMemoryBarrier BufferMemoryBarrier()
			{
				VkBufferMemoryBarrier bufferMemoryBarrier = {};
				bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
				bufferMemoryBarrier.pNext = NULL;
				return bufferMemoryBarrier;
			}

			VkMemoryBarrier MemoryBarrier()
			{
				VkMemoryBarrier memoryBarrier = {};
				memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
				memoryBarrier.pNext = NULL;
				return memoryBarrier;
			}

			VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlags usageFlags, const VkCommandBufferInheritanceInfo* inheritanceInfo)
			{
				VkCommandBufferBeginInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				info.pNext = nullptr;
				info.flags = usageFlags;
				info.pInheritanceInfo = inheritanceInfo;
				return info;
			}
		} // namespace VkUtils::Initializers

		void SetImageLayout(
			VkCommandBuffer cmdbuffer,
			VkImage image,
			VkImageAspectFlags aspectMask,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout,
			VkImageSubresourceRange subresourceRange)
		{
			// Create an image barrier object
			VkImageMemoryBarrier imageMemoryBarrier = VkUtils::Initializers::ImageMemoryBarrier();
			imageMemoryBarrier.oldLayout = oldImageLayout;
			imageMemoryBarrier.newLayout = newImageLayout;
			imageMemoryBarrier.image = image;
			imageMemoryBarrier.subresourceRange = subresourceRange;

			// Source layouts (old)

			// Undefined layout
			// Only allowed as initial layout!
			// Make sure any writes to the image have been finished
			if (oldImageLayout == VK_IMAGE_LAYOUT_PREINITIALIZED)
			{
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
			}

			if (oldImageLayout == VK_IMAGE_LAYOUT_UNDEFINED)
			{
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			}

			// Old layout is color attachment
			// Make sure any writes to the color buffer have been finished
			if (oldImageLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
			{
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			}

			// Old layout is depth/stencil attachment
			// Make sure any writes to the depth/stencil buffer have been finished
			if (oldImageLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
			{
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			}

			// Old layout is transfer source
			// Make sure any reads from the image have been finished
			if (oldImageLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
			{
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			}

			// Old layout is shader read (sampler, input attachment)
			// Make sure any shader reads from the image have been finished
			if (oldImageLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			{
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			}

			// Target layouts (new)

			// New layout is transfer destination (copy, blit)
			// Make sure any copies to the image have been finished
			if (newImageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			}

			// New layout is transfer source (copy, blit)
			// Make sure any reads from and writes to the image have been finished
			if (newImageLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
			{
				imageMemoryBarrier.srcAccessMask = imageMemoryBarrier.srcAccessMask | VK_ACCESS_TRANSFER_READ_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			}

			// New layout is color attachment
			// Make sure any writes to the color buffer have been finished
			if (newImageLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
			{
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			}

			// New layout is depth attachment
			// Make sure any writes to depth/stencil buffer have been finished
			if (newImageLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
			{
				imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			}

			// New layout is shader read (sampler, input attachment)
			// Make sure any writes to the image have been finished
			if (newImageLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			{
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			}

			// Put barrier on top
			VkPipelineStageFlags srcStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			VkPipelineStageFlags destStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

			// Put barrier inside setup command buffer
			vkCmdPipelineBarrier(
				cmdbuffer,
				srcStageFlags,
				destStageFlags,
				0,
				0, nullptr,
				0, nullptr,
				1, &imageMemoryBarrier);
		}

		void SetImageLayout(
			VkCommandBuffer cmdbuffer,
			VkImage image,
			VkImageAspectFlags aspectMask,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout)
		{
			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.aspectMask = aspectMask;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = 1;
			subresourceRange.layerCount = 1;
			SetImageLayout(cmdbuffer, image, aspectMask, oldImageLayout, newImageLayout, subresourceRange);
		}
	} // namespace VkUtils
}