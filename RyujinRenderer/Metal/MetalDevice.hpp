#pragma once


#include "RyujinCore/Logger/Logger.hpp"
#include "RyujinCore/Window/Window.hpp"
#include "RyujinCore/CoreUtils/Singleton.hpp"
#include "RyujinCore/CoreUtils/SmartPointer.hpp"
#include "RyujinCore/Containers/HashTable.hpp"

#include "../Renderer/SceneView.hpp"
#include "RyujinCore/Math/Transform.hpp"

#include "../Renderer/Buffer.hpp"

#include "MetalCommandQueue.hpp"
#include "../Common/ResourceDescriptors.hpp"

#if PLATFORM_MAC

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>


namespace Ryujin
{
    enum DeviceCapFeature
    {
        DC_MaxVertexAttribs = 0,
        DC_MaxBufferArgumentTableSize,
        DC_MaxSamplerStateArgumentTableSize,
        DC_MaxDataBlockSizeForFunction,
        DC_MaxShaderInterpolators,
        DC_MaxTextureLayersPer1DOr2DTextureArray,
        DC_MaxTextureLayersPer3DTextureArray,
        DC_MaxPointSize,
        DC_MaxVisibilityQueryOffset, // for setVisibilityResultMode::offset
        DC_Max3DTextureWidth,
        DC_Max3DTextureHeight = DC_Max3DTextureWidth,
        DC_Max3DTextureDepth = DC_Max3DTextureWidth,
        DC_MaxBufferLength,
        DC_MaxNumberOfScalarInputs, // only for fragment shader?
        DC_MaxTextureEntriesPerRender,
        DC_MaxThreadsPerThreadGroup,
        DC_MaxTotalThreadGroupMemAllocation,
        DC_MinBufferOffsetAlignmentForShader,
        DC_MaxCubeMapsInCubeMapArray,
        DC_MaxMemAllocationForShaderInConstantAddressSpace,
        DC_BufferAlignmentForTextureFromBuffer,
        DC_BufferAlignmentForCopyingTextureToBuffer,
        DC_MaxColorRenderTargetsPerRenderPass,
        DC_MaxTotalRenderTargetSize,
        DC_Max1DTextureWidth,
        DC_Max2DTextureWidth,
        DC_Max2DTextureHeight = DC_Max2DTextureWidth,
        DC_MaxCubeMapTextureWidth,
        DC_MaxCubeMapTextureHeight = DC_MaxCubeMapTextureWidth
    };
    
    // https://developer.apple.com/library/mac/documentation/Miscellaneous/Conceptual/MetalProgrammingGuide/MetalFeatureSetTables/MetalFeatureSetTables.html
    static const uint32 OSX_GPUFamily1_v1[] = {
        31,
        31,
        16,
        4096,
        60,
        2048,
        2048,
        511,
        65528,
        2048,
        256 * 1024 * 1024, // MB
        128,
        32,
        128,
        1024,
        32 * 1024,
        256,
        341,
        64 * 1024,
        0,
        256,
        8,
        (uint32)-1, // no limit
        16384,
        16384,
        16384
    };
    
    class MetalDevice
    {
    protected:
        id<MTLDevice> device;
        const char* gpuName;
        
        HashTable<uint64, id<MTLDepthStencilState>> cachedDepthStencilStates;
        HashTable<uint64, class MetalSampler*> cachedSamplers;
        
        Window* windowRef;
        bool bIsInitialized = false;
        
        MetalCommandQueue commandQueue;
        class MetalSwapChain* swapChain;
        
    public:
        static MetalDevice* Instance();
        
        bool CreateDevice();
        void Shutdown();
        bool PostInit(const uint32 width, const uint32 height);
        void OnWindowResize(const uint32 width, const uint32 height);
        
        void RunTest();
        
        VertexBufferPtr NewVertexBuffer(const void* data, uint32 elementSize, uint32 count);
        IndexBufferPtr NewIndexBuffer(const void* data, uint32 elementSize, uint32 count, bool bIs16Bit);
        ConstantBufferPtr NewConstantBuffer(const void* data, uint64 bytes);
        
        MetalPipelineState* NewPipelineState(class MetalShaderModule* library, const char* vertName, const char* fragName);
        MetalPipelineState* NewPipelineState(class MetalShaderModule* library, const RenderPipelineStateDescriptor& rpd);
        
        class MetalTexture* NewTexture(uint16 width, uint16 height, PixelFormat format, bool mipmapped, TextureUsage usage);
        class MetalTexture* NewTexture(const TextureDescriptor& desc);
        class MetalCubemap* NewTextureCube(const TextureDescriptor& desc);
        class MetalCubemapArray* NewTextureCubeArray(const TextureDescriptor& desc);
        class MetalTextureArray* NewTextureArray(const TextureDescriptor& desc);
        class MetalSampler* GetOrCreateSampler(SamplerDescriptor& desc);
        void CopyTextureToBackBuffer(class MetalTexture* src, class MetalBackBufferImage* dst);
        
        
        id<MTLDepthStencilState> GetOrCreateDepthStencilState(CompareFunction compareFunc, bool bDepthWriteEnabled,
                                                              CompareFunction frontStencilComp = CF_Never, StencilOperation frontStencilFailure = SO_Keep,
                                                              StencilOperation frontDepthFailure = SO_Keep, StencilOperation frontDepthStencilPass = SO_Keep,
                                                              uint32 frontReadMask = 0xffffffff, uint32 frontWriteMask = 0xffffffff,
                                                              CompareFunction backStencilComp = CF_Never, StencilOperation backStencilFailure = SO_Keep,
                                                              StencilOperation backDepthFailure = SO_Keep, StencilOperation backDepthStencilPass = SO_Keep,
                                                              uint32 backReadMask = 0xffffffff, uint32 backWriteMask = 0xffffffff);
        
        uint32 QueryDeviceCapFeature(const DeviceCapFeature feature);
        void PrintDeviceInfo(const bool bPrintEverything = false);
        
        FORCEINLINE id<MTLDevice> GetMetalDevice() const { return device; }
        FORCEINLINE MetalCommandQueue* GetImmediateCmdQueue() { return &commandQueue; }
        FORCEINLINE MetalSwapChain* GetSwapChain() const { return swapChain; }
        FORCEINLINE const char* GetGpuName() const { return gpuName; }
        FORCEINLINE void CacheWindowReference(Window* inWindow) { windowRef = inWindow; }
        FORCEINLINE bool IsInitialized() const { return bIsInitialized; }
    };
}

#endif //PLATFORM_MAC

