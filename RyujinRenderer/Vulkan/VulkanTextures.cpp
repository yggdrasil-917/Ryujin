#include "VulkanTextures.hpp"
#include "VulkanCommandQueue.hpp"

namespace Ryujin
{
	bool VulkanTexture::Allocate(const TextureDescriptor& desc)
	{
		return true;
	}
	void VulkanTexture::Allocate(uint16 width, uint16 height, PixelFormat format, bool mipmapped, TextureUsage usage)
	{

	}
	void VulkanTexture::UpdateTexture(Rect<uint16> region, uint8* data, uint8 mip, uint8 slice)
	{

	}
	void VulkanTexture::GenerateMips(VulkanCommandQueue* queue)
	{

	}

	void VulkanSampler::Create(SamplerDescriptor& desc)
	{

	}
}