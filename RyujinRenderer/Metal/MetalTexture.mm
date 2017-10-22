#include "MetalTexture.hpp"

#include "MetalDevice.hpp"
#include "MetalRendererCommon.hpp"

#include "../RenderUtils/TextureUtils.hpp"

namespace Ryujin
{
    bool MetalTexture::Allocate(const TextureDescriptor& desc)
    {
        MTLTextureDescriptor *descriptor = nil;
        if (bIsCubemap)
        {
            // Allocate for cubemap
            if (desc.arrayLength > 1)
            {
                descriptor.textureType = MTLTextureTypeCubeArray;
                descriptor.width = desc.width;
                descriptor.height = desc.width;
                descriptor.mipmapLevelCount = desc.mipmapped ? (MathUtils::Log2(desc.width)) : 1;
            }
            else
            {
                descriptor = [MTLTextureDescriptor textureCubeDescriptorWithPixelFormat:gPixelFormat[desc.pixelFormat]
                                                                                   size:desc.width
                                                                              mipmapped:desc.mipmapped];
            }
        }
        else if (desc.arrayLength > 1)
        {
            // Create non cubemap texture array
            descriptor.textureType = desc.height > 1 ? MTLTextureType2DArray : MTLTextureType1DArray;
            descriptor.width = desc.width;
            descriptor.height = desc.height;
            uint16 maxSize = MathUtils::Max(desc.width, desc.height);
            descriptor.mipmapLevelCount = desc.mipmapped ? (MathUtils::Log2(maxSize)) : 1;
        }
        else
        {
            // Regular 1D/2D/3D texture
            if (desc.depth > 1)
            {
                descriptor.textureType = MTLTextureType3D;
                descriptor.width = desc.width;
                descriptor.height = desc.width;
                descriptor.depth = desc.depth;
                uint16 maxSize = MathUtils::Max(MathUtils::Max(desc.width, desc.height), desc.depth);
                descriptor.mipmapLevelCount = desc.mipmapped ? (MathUtils::Log2(maxSize)) : 1;
            }
            else if (desc.height > 1)
            {
                descriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:gPixelFormat[desc.pixelFormat]
                                                                                width:desc.width
                                                                               height:desc.height
                                                                            mipmapped:(desc.sampleCount > 1 ? NO : desc.mipmapped)];
                if (desc.sampleCount > 1)
                {
                    descriptor.textureType = MTLTextureType2DMultisample;
                }
            }
            else
            {
                descriptor.textureType = MTLTextureType1D;
                descriptor.width = desc.width;
                descriptor.mipmapLevelCount = desc.mipmapped ? (MathUtils::Log2(desc.width)) : 1;
            }
        }
        
        descriptor.sampleCount = descriptor.textureType != MTLTextureType2DMultisample ? 1 : desc.sampleCount;
        
        if (desc.usage & TU_BackBuffer)
        {
            descriptor.storageMode = MTLStorageModePrivate;
            descriptor.usage = MTLTextureUsageRenderTarget;
        }
        else if (desc.usage & TU_RenderTarget)
        {
            if (desc.usage & (TU_ShaderRead | TU_ShaderWrite))
            {
                descriptor.storageMode = MTLStorageModeManaged;
            }
            else
            {
                descriptor.storageMode = MTLStorageModePrivate;
            }
            descriptor.usage = MTLTextureUsageRenderTarget;
        }
        else
        {
            if (desc.usage & TU_ShaderWrite)
            {
                descriptor.usage |= MTLTextureUsageShaderWrite;
            }
            if (desc.usage & TU_ShaderWrite)
            {
                descriptor.usage |= MTLTextureUsageShaderRead;
            }
            descriptor.storageMode = MTLStorageModeManaged;
        }
        
        texture = [MetalDevice::Instance()->GetMetalDevice() newTextureWithDescriptor:descriptor];
        width = desc.width;
        height = desc.height;
        depth = desc.depth;
        arraySize = desc.arrayLength;
        format = desc.pixelFormat;
        
        return true;
    }
    
    void MetalTexture::Allocate(uint16 width, uint16 height, PixelFormat format, bool mipmapped, TextureUsage usage)
    {
        MTLTextureDescriptor *descriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:gPixelFormat[format]
                                                                                              width:width
                                                                                             height:height
                                                                                          mipmapped:mipmapped];
        if (usage & TU_BackBuffer)
        {
            descriptor.storageMode = MTLStorageModePrivate;
            descriptor.usage = MTLTextureUsageRenderTarget;
        }
        else if (usage & TU_RenderTarget)
        {
            if (usage & (TU_ShaderRead | TU_ShaderWrite))
            {
                descriptor.storageMode = MTLStorageModeManaged;
            }
            else
            {
                descriptor.storageMode = MTLStorageModePrivate;
            }
            descriptor.usage = MTLTextureUsageRenderTarget;
        }
        else
        {
            if (usage & TU_ShaderWrite)
            {
                descriptor.usage |= MTLTextureUsageShaderWrite;
            }
            if (usage & TU_ShaderWrite)
            {
                descriptor.usage |= MTLTextureUsageShaderRead;
            }
            descriptor.storageMode = MTLStorageModeManaged;
        }
        texture = [MetalDevice::Instance()->GetMetalDevice() newTextureWithDescriptor:descriptor];
        this->width = width;
        this->height = height;
        this->depth = 1;
        this->format = format;
    }
    
    void MetalTexture::UpdateTexture(Rect<uint16> region, uint8* data, uint8 mip, uint8 slice)
    {
        MTLRegion mtlRegion = MTLRegionMake2D(region.x, region.y, region.width, region.height);
        uint64 pitch = TextureUtils::GetBytesPerPixel(format) * (width >> mip);
        [texture replaceRegion:mtlRegion mipmapLevel:mip slice:slice withBytes:data bytesPerRow:pitch bytesPerImage:(pitch * width)];
//        [texture replaceRegion:mtlRegion mipmapLevel:mip withBytes:data bytesPerRow:pitch];
    }
    
    void MetalTexture::GenerateMips(MetalCommandQueue* queue)
    {
        MetalCommandBuffer* buffer = queue->NewCommandBuffer();
        id<MTLBlitCommandEncoder> commandEncoder = [buffer->GetHandle() blitCommandEncoder];
        [commandEncoder generateMipmapsForTexture:texture];
        [commandEncoder endEncoding];
        buffer->Commit();
        buffer->WaitUntilDone();
        delete buffer;
    }
    
    void MetalSampler::Create(SamplerDescriptor& desc)
    {
        MTLSamplerDescriptor *samplerDescriptor = [MTLSamplerDescriptor new];
        samplerDescriptor.minFilter = gMinMagFilter[desc.minFilter];
        samplerDescriptor.magFilter = gMinMagFilter[desc.magFilter];
        samplerDescriptor.rAddressMode = gAddressMode[desc.r];
        samplerDescriptor.sAddressMode = gAddressMode[desc.s];
        samplerDescriptor.tAddressMode = gAddressMode[desc.t];
        sampler = [MetalDevice::Instance()->GetMetalDevice() newSamplerStateWithDescriptor:samplerDescriptor];
    }
}
