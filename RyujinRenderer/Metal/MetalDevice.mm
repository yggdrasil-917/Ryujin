

#include "MetalDevice.hpp"
#include "MetalRendererCommon.hpp"
#include "MetalSwapChain.hpp"
#include "MetalPipelineState.hpp"
#include "MetalShaderModule.hpp"
#include "MetalShader.hpp"

#include "../Renderer/Buffer.hpp"
#include "../Renderer/RendererCommon.hpp"


namespace Ryujin
{
    MetalDevice* MetalDevice::Instance()
    {
        static MetalDevice instance;
        return &instance;
    }
    
    bool MetalDevice::CreateDevice()
    {
        device = MTLCreateSystemDefaultDevice();
        
        bIsInitialized = true;
        
        gpuName = [[device name] UTF8String];
        
        commandQueue.Create();
        
        swapChain = RYUJIN_NEW MetalSwapChain();
        swapChain->Create(640, 320, 2);
        
        return true;
    }
    
    void MetalDevice::OnWindowResize(const uint32 width, const uint32 height)
    {
        
    }
    
    void MetalDevice::Shutdown()
    {
        [device release];
        bIsInitialized = false;
    }
    
    VertexBufferPtr MetalDevice::NewVertexBuffer(const void *data, uint32 elementSize, uint32 count)
    {
        RYUJIN_ASSERT(bIsInitialized, "Initialize graphics device before creating resources");
        VertexBufferPtr buffer(RYUJIN_NEW VertexBuffer());
        buffer->Create(data, elementSize, count);
        return buffer;
    }
    
    IndexBufferPtr MetalDevice::NewIndexBuffer(const void *data, uint32 elementSize, uint32 count, bool bIs16Bit)
    {
        RYUJIN_ASSERT(bIsInitialized, "Initialize graphics device before creating resources");
        IndexBufferPtr buffer(RYUJIN_NEW IndexBuffer());
        buffer->Create(data, elementSize, count, bIs16Bit);
        return buffer;
    }
    
    ConstantBufferPtr MetalDevice::NewConstantBuffer(const void *data, uint64 bytes)
    {
        RYUJIN_ASSERT(bIsInitialized, "Initialize graphics device before creating resources");
        ConstantBufferPtr buffer(RYUJIN_NEW ConstantBuffer());
        buffer->Create(data, bytes);
        return buffer;
    }
    
    MetalPipelineState* MetalDevice::NewPipelineState(MetalShaderModule *library, const char *vertName, const char *fragName)
    {
        MetalGraphicsShader vert;
        MetalGraphicsShader frag;
        if (library->InitNewGraphicsShader(vert, vertName) == false ||
            library->InitNewGraphicsShader(frag, fragName) == false)
        {
            return nullptr;
        }
        
        MTLRenderPipelineDescriptor* pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        [pipelineStateDescriptor setVertexFunction:vert.GetHandle()];
        [pipelineStateDescriptor setFragmentFunction:frag.GetHandle()];
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        pipelineStateDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        NSError* err = NULL;
        id<MTLRenderPipelineState> handle = [device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&err];
        if (handle == nil)
        {
            return nullptr;
        }
        
        MetalPipelineState* pipelineState = RYUJIN_NEW MetalPipelineState();
        pipelineState->pipelineState = handle;
        [pipelineStateDescriptor release];
        
        return pipelineState;
    }
    
    MetalPipelineState* MetalDevice::NewPipelineState(MetalShaderModule *library, const RenderPipelineStateDescriptor& rpd)
    {
        MetalGraphicsShader vert;
        MetalGraphicsShader frag;
        if (library->InitNewGraphicsShader(vert, rpd.vertexShaderName) == false ||
            library->InitNewGraphicsShader(frag, rpd.fragShaderName) == false)
        {
            return nullptr;
        }
        
        MTLRenderPipelineDescriptor* pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        [pipelineStateDescriptor setVertexFunction:vert.GetHandle()];
        [pipelineStateDescriptor setFragmentFunction:frag.GetHandle()];
        
        if (rpd.vertexDescriptor.vertAttributeCount > 0)
        {
            MTLVertexDescriptor *vertexDescriptor = [MTLVertexDescriptor new];
            for (uint32 i = 0; i < rpd.vertexDescriptor.vertAttributeCount; ++i)
            {
                vertexDescriptor.attributes[i].bufferIndex = rpd.vertexDescriptor.vertexAttributes[i].index;
                vertexDescriptor.attributes[i].offset = rpd.vertexDescriptor.vertexAttributes[i].offset;
                vertexDescriptor.attributes[i].format = gVertexFormat[rpd.vertexDescriptor.vertexAttributes[i].type];
            }
            
            vertexDescriptor.layouts[0].stepFunction = rpd.vertexDescriptor.perVertex ? MTLVertexStepFunctionPerVertex : MTLVertexStepFunctionPerInstance;
            vertexDescriptor.layouts[0].stepRate = 1;
            vertexDescriptor.layouts[0].stride = rpd.vertexDescriptor.stride;
            pipelineStateDescriptor.vertexDescriptor = vertexDescriptor;
        }
        
        for (uint8 i = 0; i < rpd.colorAttachmentCount; ++i)
        {
            pipelineStateDescriptor.colorAttachments[i].pixelFormat = gPixelFormat[rpd.colorAttachment[i].format];
        }
        pipelineStateDescriptor.depthAttachmentPixelFormat = gPixelFormat[rpd.depthFormat];// MTLPixelFormatDepth32Float;
        
        NSError* err = NULL;
        id<MTLRenderPipelineState> handle = [device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&err];
        if (handle == nil)
        {
            return nullptr;
        }
        
        MetalPipelineState* pipelineState = RYUJIN_NEW MetalPipelineState();
        pipelineState->pipelineState = handle;
        [pipelineStateDescriptor release];
        
        return pipelineState;
    }
    
    MetalTexture* MetalDevice::NewTexture(uint16 width, uint16 height, PixelFormat format, bool mipmapped, TextureUsage usage)
    {
        MetalTexture* t = RYUJIN_NEW MetalTexture();
        t->Allocate(width, height, format, mipmapped, usage);
        return t;
    }
    
    MetalTexture* MetalDevice::NewTexture(const TextureDescriptor& desc)
    {
        MetalTexture* t = RYUJIN_NEW MetalTexture();
        t->Allocate(desc);
        return t;
    }
    
    MetalCubemap* MetalDevice::NewTextureCube(const TextureDescriptor& desc)
    {
        MetalCubemap* t = RYUJIN_NEW MetalCubemap();
        t->Allocate(desc);
        return t;
    }
    
    MetalCubemapArray* MetalDevice::NewTextureCubeArray(const TextureDescriptor& desc)
    {
        MetalCubemapArray* t = RYUJIN_NEW MetalCubemapArray();
        t->Allocate(desc);
        return t;
    }
    
    MetalTextureArray* MetalDevice::NewTextureArray(const TextureDescriptor& desc)
    {
        MetalTextureArray* t = RYUJIN_NEW MetalTextureArray();
        t->Allocate(desc);
        return t;
    }
    
    MetalSampler* MetalDevice::GetOrCreateSampler(SamplerDescriptor& desc)
    {
        MetalSampler* sampler = nullptr;
        uint64 hash = GfxUtils::HashSampler(desc);
        if (cachedSamplers.TryGetValue(hash, sampler))
        {
            return sampler;
        }
        
        sampler = RYUJIN_NEW MetalSampler();
        sampler->Create(desc);
        cachedSamplers.Add(hash, sampler);
        
        return sampler;
    }
    
    void MetalDevice::CopyTextureToBackBuffer(MetalTexture* src, MetalBackBufferImage* dst)
    {
        MetalCommandBuffer* buffer = commandQueue.NewCommandBuffer();
        id<MTLBlitCommandEncoder> commandEncoder = [buffer->GetHandle() blitCommandEncoder];
        
        [commandEncoder
         copyFromTexture:src->GetHandle()
         sourceSlice:0
         sourceLevel:0
         sourceOrigin:MTLOriginMake(0, 0, 0)
         sourceSize:MTLSizeMake(src->GetWidth(), src->GetHeight(), src->GetDepth())
         toTexture:dst->GetHandle().texture
         destinationSlice:0
         destinationLevel:0
         destinationOrigin:MTLOriginMake(0, 0, 0)];
        
        [commandEncoder endEncoding];
        buffer->Present(dst);
        buffer->Commit();
        buffer->WaitUntilDone();
        delete buffer;
    }
    
    id<MTLDepthStencilState> MetalDevice::GetOrCreateDepthStencilState(CompareFunction compareFunc, bool bDepthWriteEnabled,
                                                                       CompareFunction frontStencilComp, StencilOperation frontStencilFailure,
                                                                       StencilOperation frontDepthFailure, StencilOperation frontDepthStencilPass,
                                                                       uint32 frontReadMask, uint32 frontWriteMask,
                                                                       CompareFunction backStencilComp, StencilOperation backStencilFailure,
                                                                       StencilOperation backDepthFailure, StencilOperation backDepthStencilPass,
                                                                       uint32 backReadMask, uint32 backWriteMask)
    {
        uint64 hash = 0;
        hash = GfxUtils::HashDepthStencilState(compareFunc, bDepthWriteEnabled,
                                               frontStencilComp, frontStencilFailure, frontDepthFailure, frontDepthStencilPass, frontReadMask, frontWriteMask,
                                               backStencilComp, backStencilFailure, backDepthFailure, backDepthStencilPass, backReadMask, backWriteMask);
        id<MTLDepthStencilState> ds = nil;
        if (cachedDepthStencilStates.TryGetValue(hash, ds))
        {
            return ds;
        }
        
        MTLDepthStencilDescriptor *depthStateDesc = [[MTLDepthStencilDescriptor alloc] init];
        depthStateDesc.depthCompareFunction = gCompareFunc[compareFunc];
        depthStateDesc.depthWriteEnabled = bDepthWriteEnabled ? YES : NO;
        id<MTLDepthStencilState> _depthState = [device newDepthStencilStateWithDescriptor:depthStateDesc];
        cachedDepthStencilStates.Add(hash, _depthState);
        return _depthState;
    }
    
    void MetalDevice::PrintDeviceInfo(const bool bPrintEverything)
    {
        Debugf(Channel::Renderer, "GPU: %s", gpuName);
    }
    
    uint32 MetalDevice::QueryDeviceCapFeature(const DeviceCapFeature feature)
    {
        if ([device supportsFeatureSet:MTLFeatureSet_OSX_GPUFamily1_v1])
        {
            return OSX_GPUFamily1_v1[feature];
        }
        
        return uint32(-1);
    }
    
    bool MetalDevice::PostInit(const uint32 width, const uint32 height)
    {
        return true;
    }
    
    void MetalDevice::RunTest()
    {
    }
}
