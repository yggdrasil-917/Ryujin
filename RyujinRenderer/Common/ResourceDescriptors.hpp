#pragma once

#include "RyujinCore/Allocators/Memory.hpp"


namespace Ryujin
{
    enum BufferUsage 
    {
        BU_StaticDraw = 0,
        BU_StaticRead = 1,
        BU_StaticCopy = 2,
        
        BU_StreamDraw = 3,
    };
    
    enum BufferTarget 
    {
        BT_None					= 0,
        BT_VertexBuffer			= 1,
        BT_IndexBuffer			= 2,
        BT_UniformBuffer		= 3,
        BT_TextureBuffer		= 4,
        BT_ShaderStorageBuffer	= 5,
        BT_CommandBuffer		= 6,
    };
    
    enum VertexElementType 
    {
        VET_Float4 = 0,
        VET_Float3 = 1,
        VET_Float2 = 2,
        VET_Float  = 3,
    };
    
    enum CompareFunction 
    {
        CF_Never           = 0,
        CF_LessThan        = 1,
        CF_Equal           = 2,
        CF_LessEqual       = 3,
        CF_GreaterThan     = 4,
        CF_NotEqual        = 5,
        CF_GreaterEqual    = 6,
        CF_Always          = 7
    };
    
    enum StencilOperation 
    {
        SO_Keep            = 0,
        SO_Zero            = 1,
        SO_Replace         = 2,
        SO_IncrementClamp  = 3,
        SO_DecrementClamp  = 4,
        SO_Invert          = 5,
        SO_IncrementWrap   = 6,
        SO_DecrementWrap   = 7
    };
    
    enum BlendMode
    {
        BM_Opaque = 0,
        BM_Masked,
        BM_Translucent,
        BM_Additive
    };
    
    enum BlendOperation 
    {
        BO_Add             = 0,
        BO_Subtract        = 1,
        BO_ReverseSubtract = 2,
        BO_Min             = 3,
        BO_Max             = 4
    };
    
    enum BlendFactor 
    {
        BF_Zero                = 0,
        BF_One                 = 1,
        BF_SrcColor            = 2,
        BF_OneMinusSrcColor    = 3,
        BF_SrcAlpha            = 4,
        BF_OneMinusSrcAlpha    = 5,
        BF_DstColor            = 6,
        BF_OneMinusDstColor    = 7,
        BF_DstAlpha            = 8,
        BF_OneMinusDstAlpha    = 9,
        BF_SrcAlphaSaturated   = 10,
        BF_BlendColor          = 11,
        BF_OneMinusBlendColor  = 12,
        BF_BlendAlpha          = 13,
        BF_OneMinusBlendAlpha  = 14
    };
    
    enum ColorWriteMask 
    {
        CWM_None  = 0,
        CWM_Red   = 1,
        CWM_Green = 2,
        CWM_Blue  = 3,
        CWM_Alpha = 4,
        CWM_All   = 5
    };
    
    enum CullMode 
    {
        CM_None  = 0,
        CM_Front = 1,
        CM_Back  = 2
    };
    
    enum FaceWinding 
    {
        FW_Clockwise        = 0,
        FW_CounterClockwise = 1
    };
    
    enum TriangleFillMode 
    {
        TFM_Fill  = 0,
        TFM_Lines = 1
    };
    
    enum ClipMode 
    {
        CM_Clip  = 0,
        CM_Clamp = 1
    };
    
    enum PrimitiveType 
    {
        PT_Point           = 0,
        PT_Line            = 1,
        PT_LineStrip       = 2,
        PT_Triangle        = 3,
        PT_TriangleStrip   = 4
    };
    
    enum IndexType 
    {
        IT_Uint16 = 0,
        IT_Uint32 = 1
    };
    
    enum VisibilityResultMode 
    {
        VRM_Disabled = 0,
        VRM_Boolean  = 1,
        VRM_Counting = 2
    };
    
    enum TextureType 
    {
        TT_Type1D          = 0,
        TT_Type1DArray     = 1,
        TT_Type2D          = 2,
        TT_Type2DArray     = 3,
        TT_Type2DMS        = 4,
        TT_TypeCube        = 5,
        TT_Type3D          = 6
    };
    
    enum TextureUsage 
    {
        TU_Unknown             = 0x00,
        TU_ShaderRead          = 0x01,
        TU_ShaderWrite         = 0x02,
        TU_RenderTarget        = 0x04,
        TU_PixelFormatView     = 0x08,
        TU_BackBuffer          = 0x10,
        
        TU_ReadWrite           = 0x03,
    };
    
    enum SamplerAddressMode 
    {
        SAM_ClampToEdge     = 0,
        SAM_Repeat          = 1,
        SAM_MirrorRepeat    = 2,
        SAM_ClampToZero     = 3
    };
    
    enum SamplerMinMagFilter 
    {
        SMMF_Nearest = 0,
        SMMF_Linear  = 1
    };
    
    enum SamplerMipFilter 
    {
        SMF_NotMipmapped    = 0,
        SMF_Nearest         = 1,
        SMF_Linear          = 2
    };
    
    enum PixelFormat 
    {
        PF_Invalid                 = 0,
        PF_A8Unorm                 = 1,
        PF_R8Unorm                 = 2,
        PF_R8Snorm                 = 3,
        PF_R8Uint                  = 4,
        PF_R8Sint                  = 5,
        PF_R16Unorm                = 6,
        PF_R16Snorm                = 7,
        PF_R16Uint                 = 8,
        PF_R16Sint                 = 9,
        PF_R16Float                = 10,
        PF_RG8Unorm                = 11,
        PF_RG8Uint                 = 12,
        PF_RG8Sint                 = 13,
        PF_R32Uint                 = 14,
        PF_R32Sint                 = 15,
        PF_R32Float                = 16,
        PF_RG16Unorm               = 17,
        PF_RG16Snorm               = 18,
        PF_RG16Uint                = 19,
        PF_RG16Sint                = 20,
        PF_RG16Float               = 21,
        PF_RGBA8Unorm              = 22,
        PF_RGBA8Unorm_sRGB         = 23,
        PF_RGBA8Snorm              = 24,
        PF_RGBA8Uint               = 25,
        PF_RGBA8Sint               = 26,
        PF_BGRA8Unorm              = 27,
        PF_BGRA8Unorm_sRGB         = 28,
        PF_RGB10A2Unorm            = 29,
        PF_RGB10A2Uint             = 30,
        PF_RG11B10Float            = 31,
        PF_RGB9E5Float             = 32,
        PF_RG32Uint                = 33,
        PF_RG32Sint                = 34,
        PF_RG32Float               = 35,
        PF_RGBA16Unorm             = 36,
        PF_RGBA16Snorm             = 37,
        PF_RGBA16Uint              = 38,
        PF_RGBA16Sint              = 39,
        PF_RGBA16Float             = 40,
        PF_RGBA32Uint              = 41,
        PF_RGBA32Sint              = 42,
        PF_RGBA32Float             = 43,
        PF_Depth32Float            = 44,
        PF_Depth32Float_Stencil8   = 45,
        PF_Stencil8                = 46,
        
        PF_Count
    };
    
    static const char* pixelFormatString[] =
    {
        "Invalid"               ,
        "A8Unorm"               ,
        "R8Unorm"               ,
        "R8Snorm"               ,
        "R8Uint"                ,
        "R8Sint"                ,
        "R16Unorm"              ,
        "R16Snorm"              ,
        "R16Uint"               ,
        "R16Sint"               ,
        "R16Float"             ,
        "RG8Unorm"             ,
        "RG8Uint"              ,
        "RG8Sint"              ,
        "R32Uint"              ,
        "R32Sint"              ,
        "R32Float"             ,
        "RG16Unorm"            ,
        "RG16Snorm"            ,
        "RG16Uint"             ,
        "RG16Sint"             ,
        "RG16Float"            ,
        "RGBA8Unorm"           ,
        "RGBA8Unorm_sRGB"      ,
        "RGBA8Snorm"           ,
        "RGBA8Uint"            ,
        "RGBA8Sint"            ,
        "BGRA8Unorm"           ,
        "BGRA8Unorm_sRGB"      ,
        "RGB10A2Unorm"         ,
        "RGB10A2Uint"          ,
        "RG11B10Float"         ,
        "RGB9E5Float"          ,
        "RG32Uint"             ,
        "RG32Sint"             ,
        "RG32Float"            ,
        "RGBA16Unorm"          ,
        "RGBA16Snorm"          ,
        "RGBA16Uint"           ,
        "RGBA16Sint"           ,
        "RGBA16Float"          ,
        "RGBA32Uint"           ,
        "RGBA32Sint"           ,
        "RGBA32Float"          ,
        "Depth32Float"         ,
        "Depth32Float_Stencil8",
        "Stencil8"             ,
    };
    
    enum GfxLoadAction 
    {
        LA_Default = 0,
        LA_Load = 1,
        LA_Clear = 2,
    };
    
    enum GfxStoreAction 
    {
        SA_Default = 0,
        SA_Store = 1,
        SA_MultiSampleResolve = 2,
        SA_StoreAndMultiSampleResolve = 3,
    };
    
    struct SamplerDescriptor
    {
        SamplerAddressMode r;
        SamplerAddressMode s;
        SamplerAddressMode t;
        SamplerMinMagFilter minFilter;
        SamplerMinMagFilter magFilter;
        SamplerMipFilter mipFilter;
        int8 maxAnisotropy;
    };
    
    struct TextureDescriptor
    {
        TextureType type;
        TextureUsage usage;
        PixelFormat pixelFormat;
        int16 width;
        int16 height;
        int16 depth;
        int8 mipmapLevelCount;
        bool mipmapped;
        bool bGenerateMipsManually;
        int8 arrayLength; // only used if it is a texture array
        int8 sampleCount; // used for multi sampling
    };
#define MAX_NUM_MIPS    13
    
    enum VertexAttributeEnum
    {
        VA_POSITION = 1 << 0,
        VA_NORMAL   = 1 << 1,
        VA_UV       = 1 << 2,
        VA_UV2      = 1 << 3,
        VA_TANGENT  = 1 << 4,
    };
#define VA_Pos_Normal_UV ((VA_POSITION) | (VA_NORMAL) | (VA_UV))
    
    struct VertexAttribute
    {
        VertexElementType type;
        int8 index;
        int16 offset;
    };
    
    struct VertexDescriptor
    {
        VertexAttribute vertexAttributes[8];
        int16 vertexAttributesEnabled;
        int8 vertAttributeCount;
        int16 stride;
        bool perVertex;
    };
    
    struct BufferDescriptor
    {
        PtrSize bytesToAllocate;
        uint8* data;
        BufferUsage bufferUsage;
        BufferTarget target;
        VertexDescriptor vertexLayout;
    };
    
    struct DepthDescriptor
    {
        CompareFunction depthCompareFunction;
        bool bDepthWriteEnabled;
    };
    
    struct StencilDescriptor
    {
        StencilOperation stencilFailOp;
        StencilOperation depthFailOp;
        StencilOperation depthStencilPassOp;
        CompareFunction stencilCompareFunction;
        uint32 stencilReadMask;
        uint32 stencilWriteMask;
        float stencilFrontRefValue;
        float stencilBackRefValue;
    };
    
    struct DepthStencilDescriptor
    {
        DepthDescriptor depth;
        
        StencilDescriptor backfaceStencil;
        StencilDescriptor frontfaceStencil;
    };
    
    struct RenderStateDescriptor
    {
        DepthStencilDescriptor depthStencilDescriptor;
        CullMode cullMode;
        FaceWinding faceWinding;
        TriangleFillMode triangleFillMode;
        ClipMode clipMode;
        PrimitiveType primitiveType;
        VisibilityResultMode visibilityResultMode;
    };
    
    struct ColorAttachment
    {
        PixelFormat format;
        
        ColorAttachment()
        : format(PF_BGRA8Unorm)
        {
            
        }
    };
    
    struct RenderPipelineStateDescriptor
    {
        const char* label;
        const char* vertexShaderName;
        const char* fragShaderName;
        
        VertexDescriptor vertexDescriptor;
        ColorAttachment colorAttachment[8];
        uint8 colorAttachmentCount = 1;
        PixelFormat depthFormat;
        
        bool bUseAlphaToCoverage;
    };
    
    struct TextureAttachment
    {
        class TextureBase* texture = nullptr;
        GfxLoadAction loadAction;
        GfxStoreAction storeAction;
        float clearColor[4] = { 0.5, 0.5, 0.5, 1.0 };
    };
    
    struct RenderPassDescriptor
    {
        TextureAttachment colorAttachments[8];
        TextureAttachment depthAttachment;
    };
    
    
}
