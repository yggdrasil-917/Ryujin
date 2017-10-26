#pragma once


#include "../Common/ResourceDescriptors.hpp"
#include "RendererCommon.hpp"
#include "RyujinCore/CoreUtils/SmartPointer.hpp"

namespace Ryujin
{
#if PLATFORM_MAC
#define _CommandEncoder MetalCommandEncoder
#define _GfxEncoder MetalGfxEncoder
#define _ComputeEncoder MetalComputeEncoder
#define _CommandBuffer MetalCommandBuffer
#define _CommandQueue MetalCommandQueue
#define _PipelineState MetalPipelineState
#define _ComputePipelineState MetalComputePipelineState
#define _GraphicsShader MetalGraphicsShader
#define _ComputeShader MetalComputeShader
#define _ShaderModule MetalShaderModule
#define _BackBufferImage MetalBackBufferImage
#define _SwapChain MetalSwapChain
#define _Device MetalDevice
#define _Texture MetalTexture
#define _TextureCube MetalCubemap
#define _TextureArrayCube MetalCubemapArray
#define _TextureArray MetalTextureArray
#define _Sampler MetalSampler
#elif PLATFORM_WINDOWS
#define _CommandEncoder VulkanCommandEncoder
#define _GfxEncoder VulkanGfxEncoder
#define _ComputeEncoder VulkanComputeEncoder
#define _CommandBuffer VulkanCommandBuffer
#define _CommandQueue VulkanCommandQueue
#define _PipelineState VulkanPipelineState
#define _ComputePipelineState VulkanComputePipelineState
#define _GraphicsShader VulkanGraphicsShader
#define _ComputeShader VulkanComputeShader
#define _ShaderModule VulkanShaderModule
#define _BackBufferImage VulkanBackBufferImage
#define _SwapChain VulkanSwapChain
#define _Device VulkanDevice
#define _Texture VulkanTexture
#define _TextureCube VulkanCubemap
#define _TextureArrayCube VulkanCubemapArray
#define _TextureArray VulkanTextureArray
#define _Sampler VulkanSampler
#endif
    
//    class RENDER_API CommandEncoder : public _CommandEncoder
//    {
//        
//    };
#define CommandEncoder _CommandEncoder
    using CommandEncoderPtr = SharedPtrConcurrent<CommandEncoder>;
    
#define GfxEncoder _GfxEncoder
    using GfxEncoderPtr = SharedPtrConcurrent<GfxEncoder>;
    
#define ComputeEncoder _ComputeEncoder
    using ComputeEncoderPtr = SharedPtrConcurrent<ComputeEncoder>;
    
//    class RENDER_API CommandBuffer : public _CommandBuffer
//    {
//
//    };
#define CommandBuffer _CommandBuffer
    using CommandBufferPtr = SharedPtrConcurrent<CommandBuffer>;
    
//    class RENDER_API CommandQueue : public _CommandQueue
//    {
//        
//    };
#define CommandQueue _CommandQueue
    using CommandQueuePtr = SharedPtrConcurrent<CommandQueue>;
    
//    class RENDER_API PipelineState : public _PipelineState
//    {
//        
//    };
#define PipelineState _PipelineState
    using PipelineStatePtr = SharedPtrConcurrent<PipelineState>;
    
#define ComputePipelineState _ComputePipelineState
    using ComputePipelineStatePtr = SharedPtrConcurrent<ComputePipelineState>;
    
//    class RENDER_API GraphicsShader : public _GraphicsShader
//    {
//        
//    };
#define GraphicsShader _GraphicsShader
    using GraphicsShaderPtr = SharedPtrConcurrent<GraphicsShader>;
    
//    class RENDER_API ComputeShader : public _ComputeShader
//    {
//        
//    };
#define ComputeShader _ComputeShader
    using ComputeShaderPtr = SharedPtrConcurrent<ComputeShader>;
    
//    class RENDER_API ShaderModule : public _ShaderModule
//    {
//        
//    };
#define ShaderModule _ShaderModule
    using ShaderModulePtr = SharedPtrConcurrent<ShaderModule>;
    
//    class RENDER_API BackBufferImage : public _BackBufferImage
//    {
//        
//    };
#define BackBufferImage _BackBufferImage
    using BackBufferImagePtr = SharedPtrConcurrent<BackBufferImage>;
    
//    class RENDER_API SwapChain : public _SwapChain
//    {
//        
//    };
#define SwapChain _SwapChain
    using SwapChainPtr = SharedPtrConcurrent<SwapChain>;
    
#define Texture _Texture
    using TexturePtr = SharedPtrConcurrent<Texture>;
    
#define TextureCube _TextureCube
    using TextureCubePtr = SharedPtrConcurrent<TextureCube>;
    
#define TextureArrayCube _TextureArrayCube
    using TextureCubeArrayPtr = SharedPtrConcurrent<TextureArrayCube>;
    
#define TextureArray _TextureArray
    using TextureArrayPtr = SharedPtrConcurrent<TextureArray>;
    
#define Sampler _Sampler
    using SamplerPtr = SharedPtrConcurrent<Sampler>;
    
    class RENDER_API GfxDevice : public _Device
    {
    public:
        static _Device* GetDevice() { return _Device::Instance(); }
    };
//    using GfxDevicePtr = SharedPtrConcurrent<GfxDevice>;
    
    template<class ApiBuffer>
    class GpuBuffer
    {
    private:
        uint32 bytesAllocated;
        BufferUsage usage;
        SharedPtrConcurrent<ApiBuffer> apiBuffer;
        
    public:
        GpuBuffer(uint32 inBytesAllocated, BufferUsage inUsage, ApiBuffer* inApiBuffer)
        : bytesAllocated(inBytesAllocated),
          usage(inUsage),
          apiBuffer(inApiBuffer)
        {
            
        }
        
        ~GpuBuffer()
        {
            bytesAllocated = 0;
        }
        
        FORCEINLINE uint32 GetByteSize() const { return bytesAllocated; }
        FORCEINLINE BufferUsage GetUsage() const { return usage; }
    };
    DECLARE_RESOURCE_TYPES(GpuBuffer, Buffer);
    
    
    template<class ApiTexture>
    class GpuTexture
    {
    private:
        int16 width;
        int16 height;
        int16 depth;
        TextureType type;
        PixelFormat pixelFormat;
        SharedPtrConcurrent<ApiTexture> apiTexture;
        
    public:
        GpuTexture(const TextureDescriptor& desc, ApiTexture* inApiTexture)
        : width(desc.width),
          height(desc.height),
          depth(desc.depth),
          type(desc.type),
          pixelFormat(desc.pixelFormat),
          apiTexture(inApiTexture)
        {
            
        }
        
        ~GpuTexture()
        {
            
        }
    };
    DECLARE_RESOURCE_TYPES(GpuTexture, Texture);
    
    template<class ApiSampler>
    class GpuSampler
    {
    private:
        SharedPtrConcurrent<ApiSampler> apiSampler;
        
    public:
        GpuSampler(ApiSampler* inApiSampler)
        : apiSampler(inApiSampler)
        {
            
        }
        
        ~GpuSampler()
        {
            
        }
    };
    DECLARE_RESOURCE_TYPES(GpuSampler, Sampler);
}


