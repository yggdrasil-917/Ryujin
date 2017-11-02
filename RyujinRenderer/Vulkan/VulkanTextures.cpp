#include "VulkanTextures.hpp"
#include "VulkanCommandQueue.hpp"
#include "VulkanBuffers.hpp"

#include "VulkanRendererCommon.hpp"
#include "../RenderUtils/TextureUtils.hpp"


namespace Ryujin
{
	bool VulkanTexture::Allocate(const TextureDescriptor& desc)
	{
		width = desc.width;
		height = desc.height;
		depth = desc.depth;
		arraySize = desc.arrayLength;
		format = desc.pixelFormat;
		bHasMips = desc.mipmapped;

		VkFormatProperties formatProperties;
		VkFormat format = gPixelFormat[desc.pixelFormat];

		// Get device properties for the requested texture format
		vkGetPhysicalDeviceFormatProperties(VulkanDevice::Instance()->GetPhysicalDevice(), format, &formatProperties);

		if (desc.mipmapped)
		{
			// Mip-chain generation requires support for blit source and destination
			RYUJIN_ASSERT(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT, "Missing support for blit source");
			RYUJIN_ASSERT(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT, "Missing support for blit destination");
			return false;
		}

		stagingBuffer = RYUJIN_NEW VulkanStagingBuffer();
		uint64 size = 0;
		uint64 bytesPerPixel = TextureUtils::GetBytesPerPixel(desc.pixelFormat);
		//if (desc.mipmapped)
		//{
		//	uint32 mipCount = GetMipCount();
		//	for (uint32 i = 0; i < mipCount; ++i)
		//	{
		//		size += bytesPerPixel * MathUtils::Max(desc.width >> i, 1) * MathUtils::Max(desc.height >> i, 1);
		//	}
		//}
		//else
		{
			size = bytesPerPixel * desc.width * desc.height;
		}
		stagingBuffer->Create(nullptr, size);

		VkDevice device = VulkanDevice::Instance()->GetDevice();

		// Create optimal tiled target image
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.pNext = NULL;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = format;
		imageCreateInfo.mipLevels = GetMipCount();
		imageCreateInfo.arrayLayers = desc.arrayLength;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.extent = { width, height, depth };
		imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		VK_CHECK_RESULT(vkCreateImage(device, &imageCreateInfo, nullptr, &image));

		VkMemoryRequirements memReqs = {};
		vkGetImageMemoryRequirements(device, image, &memReqs);
		VkMemoryAllocateInfo memAllocInfo = {};
		memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memAllocInfo.pNext = NULL;
		memAllocInfo.allocationSize = memReqs.size;
		memAllocInfo.memoryTypeIndex = VulkanDevice::Instance()->GetMemoryTypeIndex(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(device, &memAllocInfo, nullptr, &mem));
		VK_CHECK_RESULT(vkBindImageMemory(device, image, mem, 0));

		return true;
	}
	void VulkanTexture::Allocate(uint16 width, uint16 height, PixelFormat format, bool mipmapped, TextureUsage usage)
	{

	}
	void VulkanTexture::UpdateTexture(Rect<uint16> region, uint8* data, uint8 mip, uint8 slice)
	{
		uint64 size = TextureUtils::GetBytesPerPixel(format) * MathUtils::Max(width >> (mip - 1), 1) * MathUtils::Max(height >> (mip - 1), 1);
		VkBufferImageCopy bufferCopyRegion = {};
		bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		bufferCopyRegion.imageSubresource.mipLevel = mip;
		bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
		bufferCopyRegion.imageSubresource.layerCount = 1;
		bufferCopyRegion.imageExtent.width = region.width;   
		bufferCopyRegion.imageExtent.height = region.height; 
		bufferCopyRegion.imageExtent.depth = 1;
		bufferCopyRegion.bufferOffset = mip == 0 ? 0 : size;

		stagingBuffer->Update(data, size);
	}
	void VulkanTexture::GenerateMips(VulkanCommandQueue* queue)
	{

	}

	VulkanSampler::~VulkanSampler()
	{
		vkDestroySampler(VulkanDevice::Instance()->GetDevice(), handle, nullptr);
	}

	void VulkanSampler::Create(SamplerDescriptor& desc)
	{
		VkSamplerCreateInfo sampler = {};
		sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler.pNext = NULL;
		sampler.magFilter = gMinMagFilter[desc.magFilter];
		sampler.minFilter = gMinMagFilter[desc.minFilter];
		sampler.mipmapMode = gSamplerMipFilter[desc.mipFilter];
		sampler.addressModeU = gAddressMode[desc.r];
		sampler.addressModeV = gAddressMode[desc.s];
		sampler.addressModeW = gAddressMode[desc.t];
		sampler.mipLodBias = 0.0f;
		sampler.compareOp = VK_COMPARE_OP_NEVER;
		sampler.minLod = 0.0f;
		sampler.maxLod = 14.0f; // 0 disables mip mapping
		sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		sampler.maxAnisotropy = desc.maxAnisotropy;
		sampler.anisotropyEnable = desc.maxAnisotropy > 1.0f ? VK_TRUE : VK_FALSE;

		VK_CHECK_RESULT(vkCreateSampler(VulkanDevice::Instance()->GetDevice(), &sampler, nullptr, &handle));
	}
}