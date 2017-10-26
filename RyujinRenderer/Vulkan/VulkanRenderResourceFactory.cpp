#include "../Renderer/RenderResourceFactory.hpp"


namespace Ryujin
{
	UniquePtr<GALBuffer> RenderResourceFactory::CreateBuffer(const BufferDescriptor& bufferDesc)
	{
		return UniquePtr<GALBuffer>(nullptr);
	}

	void RenderResourceFactory::DestroyBuffer(UniquePtr<GALBuffer>& buffer)
	{

	}


	UniquePtr<GALTexture> RenderResourceFactory::CreateTexture(const TextureDescriptor& textureDesc, ubyte* data)
	{
		return UniquePtr<GALTexture>(nullptr);
	}

	void RenderResourceFactory::DestroyTexture(UniquePtr<GALTexture>& texture)
	{

	}

	UniquePtr<GALSampler> RenderResourceFactory::CreateSampler(const SamplerDescriptor& samplerDesc)
	{
		return UniquePtr<GALSampler>(nullptr);
	}

	void RenderResourceFactory::DestroySampler(UniquePtr<GALSampler>& sampler)
	{

	}
}