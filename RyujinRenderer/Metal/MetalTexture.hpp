#pragma once


//#include "../Renderer/RendererCommon.hpp"
#include "../Common/TextureBase.hpp"
#include "../Common/ResourceDescriptors.hpp"

#if PLATFORM_MAC

#import <Metal/Metal.h>

namespace Ryujin
{
    class MetalTexture : public TextureBase
    {
    protected:
        id<MTLTexture> texture;
        
    public:
        MetalTexture()
        {
            bIsCubemap = false;
        }
        VIRTUAL ~MetalTexture() {}
        
        MetalTexture(id<MTLTexture> inTexture)
        : texture(inTexture)
        {
            
        }
        
        bool Allocate(const TextureDescriptor& desc);
        void Allocate(uint16 width, uint16 height, PixelFormat format, bool mipmapped, TextureUsage usage);
        void UpdateTexture(Rect<uint16> region, uint8* data, uint8 mip, uint8 slice = 0);
        void GenerateMips(class MetalCommandQueue* queue);
        
        FORCEINLINE id<MTLTexture> GetHandle() const { return texture; }
    };
    
    class MetalCubemap : public MetalTexture
    {
    public:
        MetalCubemap()
        {
            bIsCubemap = true;
        }
    };
    
    class MetalTextureArray : public MetalTexture
    {
        
    };
    
    class MetalCubemapArray : public MetalTexture
    {
    public:
        MetalCubemapArray()
        {
            bIsCubemap = true;
        }
    };
    
    class MetalSampler
    {
    public:
        id<MTLSamplerState> sampler;
        
        MetalSampler(id<MTLSamplerState> inSampler)
        : sampler(inSampler)
        {
            
        }
        MetalSampler() {}
        VIRTUAL ~MetalSampler() {}
        
        FORCEINLINE id<MTLSamplerState> GetHandle() const { return sampler; }
        
        void Create(SamplerDescriptor& desc);
    };
}

#endif

