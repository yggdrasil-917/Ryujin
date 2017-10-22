
#pragma once


#include "RenderResources.hpp"


namespace Ryujin
{
    class RENDER_API RenderResourceFactory
    {
    public:
        static UniquePtr<GALBuffer> CreateBuffer(const BufferDescriptor& bufferDesc);
        static void DestroyBuffer(UniquePtr<GALBuffer>& buffer);
        
        
        static UniquePtr<GALTexture> CreateTexture(const TextureDescriptor& textureDesc, ubyte* data);
        static void DestroyTexture(UniquePtr<GALTexture>& texture);
        static UniquePtr<GALSampler> CreateSampler(const SamplerDescriptor& samplerDesc);
        static void DestroySampler(UniquePtr<GALSampler>& sampler);
        
        
        static GALBuffer CreateBuffer2(const BufferDescriptor& bufferDesc);
        static GALTexture CreateTexture2(const TextureDescriptor& textureDesc, ubyte* data);
        static GALSampler CreateSampler2(const SamplerDescriptor& samplerDesc);
    };
}


