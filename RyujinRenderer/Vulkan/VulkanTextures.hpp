#pragma once


#include "../Common/TextureBase.hpp"
#include "../Common/ResourceDescriptors.hpp"
#include "VulkanBase.hpp"


namespace Ryujin
{
	class RENDER_API VulkanTexture : public TextureBase
	{
	protected:
		//VkSampler sampler;

		VkImage image;
		VkImageLayout imageLayout;

		VkDeviceMemory mem;
		VkImageView view;

	public:
		VulkanTexture()
		{
			bIsCubemap = false;
		}
		VIRTUAL ~VulkanTexture() {}

		bool Allocate(const TextureDescriptor& desc);
		void Allocate(uint16 width, uint16 height, PixelFormat format, bool mipmapped, TextureUsage usage);
		void UpdateTexture(Rect<uint16> region, uint8* data, uint8 mip, uint8 slice = 0);
		void GenerateMips(class VulkanCommandQueue* queue);

		FORCEINLINE VkImageView GetHandle() const { return view; }
	};

	class RENDER_API VulkanSampler
	{
	private:
		VkSampler handle;

	public:
		void Create(SamplerDescriptor& desc);

		FORCEINLINE VkSampler GetHandle() const { return handle; }
	};

	class RENDER_API VulkanCubemap : public VulkanTexture
	{
	public:
		VulkanCubemap()
		{
			bIsCubemap = true;
		}
	};

	class RENDER_API VulkanTextureArray : public VulkanTexture
	{

	};

	class RENDER_API VulkanCubemapArray : public VulkanTexture
	{
	public:
		VulkanCubemapArray()
		{
			bIsCubemap = true;
		}
	};
}