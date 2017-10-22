
#pragma once

#include "../Common/RenderingUtils.hpp"

#define USE_METAL 1
#include <Metal/Metal.h>

namespace Ryujin
{
    static const MTLVertexFormat gVertexFormat[] = {
        MTLVertexFormatFloat4,
        MTLVertexFormatFloat3,
        MTLVertexFormatFloat2,
        MTLVertexFormatFloat
    };
    
    static const MTLCompareFunction gCompareFunc[] = {
        MTLCompareFunctionNever,
        MTLCompareFunctionLess,
        MTLCompareFunctionEqual,
        MTLCompareFunctionLessEqual,
        MTLCompareFunctionGreater,
        MTLCompareFunctionNotEqual,
        MTLCompareFunctionGreaterEqual,
        MTLCompareFunctionAlways
    };
    
    static const MTLStencilOperation gStencilOp[] = {
        MTLStencilOperationKeep,
        MTLStencilOperationZero,
        MTLStencilOperationReplace,
        MTLStencilOperationIncrementClamp,
        MTLStencilOperationDecrementClamp,
        MTLStencilOperationInvert,
        MTLStencilOperationIncrementWrap,
        MTLStencilOperationDecrementWrap
    };
    
    static const MTLBlendOperation gBlendOp[] = {
        MTLBlendOperationAdd,
        MTLBlendOperationSubtract,
        MTLBlendOperationReverseSubtract,
        MTLBlendOperationMin,
        MTLBlendOperationMax
    };
    
    static const MTLBlendFactor gBlendFactor[] = {
        MTLBlendFactorZero,
        MTLBlendFactorOne,
        MTLBlendFactorSourceColor,
        MTLBlendFactorOneMinusSourceColor,
        MTLBlendFactorSourceAlpha,
        MTLBlendFactorOneMinusSourceAlpha,
        MTLBlendFactorDestinationColor,
        MTLBlendFactorOneMinusDestinationColor,
        MTLBlendFactorDestinationAlpha,
        MTLBlendFactorOneMinusDestinationAlpha,
        MTLBlendFactorSourceAlphaSaturated,
        MTLBlendFactorBlendColor,
        MTLBlendFactorOneMinusBlendColor,
        MTLBlendFactorBlendAlpha,
        MTLBlendFactorOneMinusBlendAlpha
    };
    
    static const MTLColorWriteMask gColorMask[] = {
        MTLColorWriteMaskNone,
        MTLColorWriteMaskRed,
        MTLColorWriteMaskGreen,
        MTLColorWriteMaskBlue,
        MTLColorWriteMaskAlpha,
        MTLColorWriteMaskAll
    };
    
    static const MTLCullMode gCullMode[] = {
        MTLCullModeNone,
        MTLCullModeFront,
        MTLCullModeBack
    };
    
    static const MTLWinding gWinding[] = {
        MTLWindingClockwise,
        MTLWindingCounterClockwise
    };
    
    static const MTLTriangleFillMode gFillMode[] = {
        MTLTriangleFillModeFill,
        MTLTriangleFillModeLines
    };
    
    static const MTLDepthClipMode gClipMode[] = {
        MTLDepthClipModeClip,
        MTLDepthClipModeClamp
    };
    
    static const MTLPrimitiveType gPrimType[] = {
        MTLPrimitiveTypePoint,
        MTLPrimitiveTypeLine,
        MTLPrimitiveTypeLineStrip,
        MTLPrimitiveTypeTriangle,
        MTLPrimitiveTypeTriangleStrip
    };
    
    static const MTLIndexType gIndexType[] = {
        MTLIndexTypeUInt16,
        MTLIndexTypeUInt32
    };
    
    static const MTLVisibilityResultMode gVisibilityMode[] = {
        MTLVisibilityResultModeDisabled,
        MTLVisibilityResultModeBoolean,
        MTLVisibilityResultModeCounting
    };
    
    static const MTLTextureType gTextureType[] = {
        MTLTextureType1D,
        MTLTextureType1DArray,
        MTLTextureType2D,
        MTLTextureType2DArray,
        MTLTextureType2DMultisample,
        MTLTextureTypeCube,
        MTLTextureType3D
    };
    
    static const MTLTextureUsage gTextureUsage[] = {
        MTLTextureUsageUnknown,
        MTLTextureUsageShaderRead,
        MTLTextureUsageShaderWrite,
        MTLTextureUsageRenderTarget,
        MTLTextureUsagePixelFormatView,
    };
    
    static const MTLSamplerAddressMode gAddressMode[] = {
        MTLSamplerAddressModeClampToEdge,
        MTLSamplerAddressModeRepeat,
        MTLSamplerAddressModeMirrorRepeat,
        MTLSamplerAddressModeClampToZero
    };
    
    static const MTLSamplerMinMagFilter gMinMagFilter[] = {
        MTLSamplerMinMagFilterNearest,
        MTLSamplerMinMagFilterLinear
    };
    
    static const MTLSamplerMipFilter gSamplerMipFilter[] = {
        MTLSamplerMipFilterNotMipmapped,
        MTLSamplerMipFilterNearest,
        MTLSamplerMipFilterLinear
    };
    
    static const MTLPixelFormat gPixelFormat[] = {
        MTLPixelFormatInvalid              ,
        MTLPixelFormatA8Unorm              ,
        MTLPixelFormatR8Unorm              ,
        MTLPixelFormatR8Snorm              ,
        MTLPixelFormatR8Uint               ,
        MTLPixelFormatR8Sint               ,
        MTLPixelFormatR16Unorm             ,
        MTLPixelFormatR16Snorm             ,
        MTLPixelFormatR16Uint              ,
        MTLPixelFormatR16Sint              ,
        MTLPixelFormatR16Float             ,
        MTLPixelFormatRG8Unorm             ,
        MTLPixelFormatRG8Uint              ,
        MTLPixelFormatRG8Sint              ,
        MTLPixelFormatR32Uint              ,
        MTLPixelFormatR32Sint              ,
        MTLPixelFormatR32Float             ,
        MTLPixelFormatRG16Unorm            ,
        MTLPixelFormatRG16Snorm            ,
        MTLPixelFormatRG16Uint             ,
        MTLPixelFormatRG16Sint             ,
        MTLPixelFormatRG16Float            ,
        MTLPixelFormatRGBA8Unorm           ,
        MTLPixelFormatRGBA8Unorm_sRGB      ,
        MTLPixelFormatRGBA8Snorm           ,
        MTLPixelFormatRGBA8Uint            ,
        MTLPixelFormatRGBA8Sint            ,
        MTLPixelFormatBGRA8Unorm           ,
        MTLPixelFormatBGRA8Unorm_sRGB      ,
        MTLPixelFormatRGB10A2Unorm         ,
        MTLPixelFormatRGB10A2Uint          ,
        MTLPixelFormatRG11B10Float         ,
        MTLPixelFormatRGB9E5Float          ,
        MTLPixelFormatRG32Uint             ,
        MTLPixelFormatRG32Sint             ,
        MTLPixelFormatRG32Float            ,
        MTLPixelFormatRGBA16Unorm          ,
        MTLPixelFormatRGBA16Snorm          ,
        MTLPixelFormatRGBA16Uint           ,
        MTLPixelFormatRGBA16Sint           ,
        MTLPixelFormatRGBA16Float          ,
        MTLPixelFormatRGBA32Uint           ,
        MTLPixelFormatRGBA32Sint           ,
        MTLPixelFormatRGBA32Float          ,
        MTLPixelFormatDepth32Float         ,
        MTLPixelFormatDepth32Float_Stencil8,
        MTLPixelFormatStencil8             ,
    };
    
    static const MTLLoadAction gLoadAction[] = {
        MTLLoadActionDontCare,
        MTLLoadActionLoad,
        MTLLoadActionClear,
    };
    
    static const MTLStoreAction gStoreAction[] = {
        MTLStoreActionDontCare,
        MTLStoreActionStore,
        MTLStoreActionMultisampleResolve,
        MTLStoreActionStoreAndMultisampleResolve,
    };
    
#define CB_ALIGNMENT 256
}



#include "MetalDevice.hpp"
#include "MetalBuffer.hpp"
#include "MetalTexture.hpp"
#include "MetalSwapChain.hpp"
#include "MetalCommandQueue.hpp"
#include "MetalShaderModule.hpp"
#include "MetalShader.hpp"
#include "MetalPipelineState.hpp"


namespace Ryujin
{
    using GALDevice                    = MetalDevice;
    using GALDevicePtr                 = MetalDevice*;
    using GALDeviceRef                 = MetalDevice&;
    
    
#define DECLARE_RESOURCE_TYPES(GpuType, ResourceType) \
    using GAL##ResourceType = GpuType<Metal##ResourceType>; \
    using GAL##ResourceType##Ptr = GpuType<Metal##ResourceType>*; \
    using GAL##ResourceType##Ref = GpuType<Metal##ResourceType>&;
    
}
