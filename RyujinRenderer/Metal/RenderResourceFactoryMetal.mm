


#include "../Renderer/RenderResourceFactory.hpp"
#include "RyujinCore/CoreUtils/ServiceLocator.hpp"
#include "../Renderer/Renderer.hpp"

#include "RyujinCore/Allocators/GlobalAllocators.hpp"
#include "../RenderUtils/TextureUtils.hpp"


namespace Ryujin
{
    UniquePtr<GALBuffer> RenderResourceFactory::CreateBuffer(const BufferDescriptor& bufferDesc)
    {
        id<MTLDevice> device = GfxDevice::GetDevice()->GetMetalDevice();
        
        id<MTLBuffer> bufferHandle = nil;
        
        if (bufferDesc.data)
        {
            bufferHandle = [device
                            newBufferWithBytes:bufferDesc.data
                            length:bufferDesc.bytesToAllocate
                            options:MTLResourceCPUCacheModeDefaultCache];
        }
        else
        {
            bufferHandle = [device
                            newBufferWithLength:bufferDesc.bytesToAllocate
                            options:MTLResourceCPUCacheModeDefaultCache];
        }
        MetalBuffer* apiBuffer = RYUJIN_NEW MetalBuffer(bufferHandle);
        
        return UniquePtr<GALBuffer>(RYUJIN_NEW GALBuffer(bufferDesc.bytesToAllocate, bufferDesc.bufferUsage, apiBuffer));
    }
    
    void RenderResourceFactory::DestroyBuffer(UniquePtr<GALBuffer>& buffer)
    {
        GALBufferPtr bufferRef = buffer.Release();
        SAFE_DELETE(bufferRef);
    }
    
    
    UniquePtr<GALTexture> RenderResourceFactory::CreateTexture(const TextureDescriptor &textureDesc, ubyte* data)
    {
        id<MTLDevice> device = GfxDevice::GetDevice()->GetMetalDevice();
        MTLTextureDescriptor* mtlTextureDesc;
        
        
        if (textureDesc.bGenerateMipsManually)
        {
            mtlTextureDesc = [[MTLTextureDescriptor new] autorelease];
            mtlTextureDesc.textureType = (MTLTextureType)textureDesc.type;
            mtlTextureDesc.pixelFormat = (MTLPixelFormat)textureDesc.pixelFormat;
            mtlTextureDesc.width = textureDesc.width;
            mtlTextureDesc.height = textureDesc.height;
            mtlTextureDesc.depth = textureDesc.depth;
            mtlTextureDesc.mipmapLevelCount = textureDesc.mipmapLevelCount;
            mtlTextureDesc.sampleCount = textureDesc.sampleCount;
            mtlTextureDesc.arrayLength = textureDesc.arrayLength;
            mtlTextureDesc.usage = (MTLTextureUsage)textureDesc.usage;
        }
        else
        {
            mtlTextureDesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:(MTLPixelFormat)textureDesc.pixelFormat
                                                                                width:textureDesc.width
                                                                               height:textureDesc.height
                                                                            mipmapped:YES];
        }
        
        id<MTLTexture> textureHandle = [device newTextureWithDescriptor:mtlTextureDesc];
        
        if (textureDesc.bGenerateMipsManually)
        {
            for (uint32 mip = 0; mip < textureDesc.mipmapLevelCount; ++mip)
            {
                const uint32 width = textureDesc.width << mip;
                const uint32 height = textureDesc.height << mip;
                MTLRegion region = MTLRegionMake2D(0, 0, width, height);
                const uint32 bytesPerPixel = TextureUtils::GetBytesPerPixel(textureDesc.pixelFormat);
                const uint32 bytesPerRow = bytesPerPixel * width;
                const uint32 bytesPerImage = bytesPerPixel * width * height;
                [textureHandle replaceRegion:region mipmapLevel:mip slice:0 withBytes:data bytesPerRow:bytesPerRow bytesPerImage:bytesPerImage];
            }
        }
        else
        {
            // Using blit command to accelerate mip generation
            
//            id<MTLCommandQueue> commandQueue = [deviceRef newCommandQueue];
//            id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
//            id<MTLBlitCommandEncoder> commandEncoder = [commandBuffer blitCommandEncoder];
//            [commandEncoder generateMipmapsForTexture:textureHandle];
//            [commandEncoder endEncoding];
//            [commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
//                //TODO: add completion handler
//                //completionBlock(texture);
//            }];
//            [commandBuffer commit];
            
            const uint32 width = textureDesc.width;
            const uint32 height = textureDesc.height;
            MTLRegion region = MTLRegionMake2D(0, 0, width, height);
            const uint32 bytesPerPixel = TextureUtils::GetBytesPerPixel(textureDesc.pixelFormat);
            const uint32 bytesPerRow = bytesPerPixel * width;
            const uint32 bytesPerImage = bytesPerPixel * width * height;
            [textureHandle replaceRegion:region mipmapLevel:0 slice:0 withBytes:data bytesPerRow:bytesPerRow bytesPerImage:bytesPerImage];
        }
        
        return UniquePtr<GALTexture>(RYUJIN_NEW GALTexture(textureDesc, RYUJIN_NEW MetalTexture(textureHandle)));
    }
    
    
    void RenderResourceFactory::DestroyTexture(UniquePtr<GALTexture>& texture)
    {
        GALTexturePtr ref = texture.Release();
        SAFE_DELETE(ref);
    }
    
    
    UniquePtr<GALSampler> RenderResourceFactory::CreateSampler(const SamplerDescriptor &samplerDesc)
    {
        id<MTLDevice> device = GfxDevice::GetDevice()->GetMetalDevice();
        MTLSamplerDescriptor *samplerDescriptor = [[MTLSamplerDescriptor new] autorelease];
        samplerDescriptor.minFilter = (MTLSamplerMinMagFilter)samplerDesc.minFilter;
        samplerDescriptor.magFilter = (MTLSamplerMinMagFilter)samplerDesc.magFilter;
        samplerDescriptor.mipFilter = (MTLSamplerMipFilter)samplerDesc.mipFilter;
        samplerDescriptor.sAddressMode = (MTLSamplerAddressMode)samplerDesc.s;
        samplerDescriptor.tAddressMode = (MTLSamplerAddressMode)samplerDesc.t;
        samplerDescriptor.rAddressMode = (MTLSamplerAddressMode)samplerDesc.r;
        samplerDescriptor.maxAnisotropy = samplerDesc.maxAnisotropy;
        
        id<MTLSamplerState> sampler = [device newSamplerStateWithDescriptor:samplerDescriptor];
        
        return UniquePtr<GALSampler>(RYUJIN_NEW GALSampler(RYUJIN_NEW MetalSampler(sampler)));
    }
    
    void RenderResourceFactory::DestroySampler(UniquePtr<GALSampler>& sampler)
    {
        GALSamplerPtr ref = sampler.Release();
        SAFE_DELETE(ref);
    }
    
    
    
    
    
    GALBuffer RenderResourceFactory::CreateBuffer2(const BufferDescriptor& bufferDesc)
    {
        id<MTLDevice> device = GfxDevice::GetDevice()->GetMetalDevice();
        id<MTLBuffer> bufferHandle = nil;
        
        if (bufferDesc.data)
        {
            bufferHandle = [device
                            newBufferWithBytes:bufferDesc.data
                            length:bufferDesc.bytesToAllocate
                            options:MTLResourceCPUCacheModeDefaultCache];
        }
        else
        {
            bufferHandle = [device
                            newBufferWithLength:bufferDesc.bytesToAllocate
                            options:MTLResourceCPUCacheModeDefaultCache];
        }
        
        return GALBuffer(bufferDesc.bytesToAllocate, bufferDesc.bufferUsage, RYUJIN_NEW MetalBuffer(bufferHandle));
    }
    
    GALTexture RenderResourceFactory::CreateTexture2(const TextureDescriptor &textureDesc, ubyte* data)
    {
        id<MTLDevice> device = GfxDevice::GetDevice()->GetMetalDevice();
        MTLTextureDescriptor* mtlTextureDesc;
        
        
        if (textureDesc.bGenerateMipsManually)
        {
            mtlTextureDesc = [[MTLTextureDescriptor new] autorelease];
            mtlTextureDesc.textureType = (MTLTextureType)textureDesc.type;
            mtlTextureDesc.pixelFormat = (MTLPixelFormat)textureDesc.pixelFormat;
            mtlTextureDesc.width = textureDesc.width;
            mtlTextureDesc.height = textureDesc.height;
            mtlTextureDesc.depth = textureDesc.depth;
            mtlTextureDesc.mipmapLevelCount = textureDesc.mipmapLevelCount;
            mtlTextureDesc.sampleCount = textureDesc.sampleCount;
            mtlTextureDesc.arrayLength = textureDesc.arrayLength;
            mtlTextureDesc.usage = (MTLTextureUsage)textureDesc.usage;
        }
        else
        {
            mtlTextureDesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:(MTLPixelFormat)textureDesc.pixelFormat
                                                                                width:textureDesc.width
                                                                               height:textureDesc.height
                                                                            mipmapped:YES];
        }
        
        id<MTLTexture> textureHandle = [device newTextureWithDescriptor:mtlTextureDesc];
        
        if (textureDesc.bGenerateMipsManually)
        {
            for (uint32 mip = 0; mip < textureDesc.mipmapLevelCount; ++mip)
            {
                const uint32 width = textureDesc.width << mip;
                const uint32 height = textureDesc.height << mip;
                MTLRegion region = MTLRegionMake2D(0, 0, width, height);
                const uint32 bytesPerPixel = TextureUtils::GetBytesPerPixel(textureDesc.pixelFormat);
                const uint32 bytesPerRow = bytesPerPixel * width;
                const uint32 bytesPerImage = bytesPerPixel * width * height;
                [textureHandle replaceRegion:region mipmapLevel:mip slice:0 withBytes:data bytesPerRow:bytesPerRow bytesPerImage:bytesPerImage];
            }
        }
        else
        {
            // Using blit command to accelerate mip generation
            
            //            id<MTLCommandQueue> commandQueue = [deviceRef newCommandQueue];
            //            id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
            //            id<MTLBlitCommandEncoder> commandEncoder = [commandBuffer blitCommandEncoder];
            //            [commandEncoder generateMipmapsForTexture:textureHandle];
            //            [commandEncoder endEncoding];
            //            [commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
            //                //TODO: add completion handler
            //                //completionBlock(texture);
            //            }];
            //            [commandBuffer commit];
            
            const uint32 width = textureDesc.width;
            const uint32 height = textureDesc.height;
            MTLRegion region = MTLRegionMake2D(0, 0, width, height);
            const uint32 bytesPerPixel = TextureUtils::GetBytesPerPixel(textureDesc.pixelFormat);
            const uint32 bytesPerRow = bytesPerPixel * width;
            const uint32 bytesPerImage = bytesPerPixel * width * height;
            [textureHandle replaceRegion:region mipmapLevel:0 slice:0 withBytes:data bytesPerRow:bytesPerRow bytesPerImage:bytesPerImage];
        }
        
        return GALTexture(textureDesc, RYUJIN_NEW MetalTexture(textureHandle));
    }
    
    GALSampler RenderResourceFactory::CreateSampler2(const SamplerDescriptor &samplerDesc)
    {
        id<MTLDevice> device = GfxDevice::GetDevice()->GetMetalDevice();
        MTLSamplerDescriptor *samplerDescriptor = [[MTLSamplerDescriptor new] autorelease];
        samplerDescriptor.minFilter = (MTLSamplerMinMagFilter)samplerDesc.minFilter;
        samplerDescriptor.magFilter = (MTLSamplerMinMagFilter)samplerDesc.magFilter;
        samplerDescriptor.mipFilter = (MTLSamplerMipFilter)samplerDesc.mipFilter;
        samplerDescriptor.sAddressMode = (MTLSamplerAddressMode)samplerDesc.s;
        samplerDescriptor.tAddressMode = (MTLSamplerAddressMode)samplerDesc.t;
        samplerDescriptor.rAddressMode = (MTLSamplerAddressMode)samplerDesc.r;
        samplerDescriptor.maxAnisotropy = samplerDesc.maxAnisotropy;
        
        id<MTLSamplerState> sampler = [device newSamplerStateWithDescriptor:samplerDescriptor];
        
        return GALSampler(RYUJIN_NEW MetalSampler(sampler));
    }
}
