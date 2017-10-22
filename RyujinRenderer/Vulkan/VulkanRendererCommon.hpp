#pragma once



#include "VulkanBase.hpp"


namespace Ryujin
{
	namespace RendererEnumsVulkan
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
			BT_None = 0,
			BT_VertexBuffer = 1,
			BT_IndexBuffer = 2,
			BT_UniformBuffer = 3,
			BT_TextureBuffer = 4,
			BT_ShaderStorageBuffer = 5,
			BT_CommandBuffer = 6,

		};

		enum VertexElementType 
		{
			VET_Float = 0,
			VET_Float2 = 1,
			VET_Float3 = 2,
			VET_Float4 = 3,
		};

		enum CompareFunction 
		{
			CF_Never = VK_COMPARE_OP_NEVER,
			CF_LessThan = VK_COMPARE_OP_LESS,
			CF_Equal = VK_COMPARE_OP_EQUAL,
			CF_LessEqual = VK_COMPARE_OP_LESS_OR_EQUAL,
			CF_GreaterThan = VK_COMPARE_OP_GREATER,
			CF_NotEqual = VK_COMPARE_OP_NOT_EQUAL,
			CF_GreaterEqual = VK_COMPARE_OP_GREATER_OR_EQUAL,
			CF_Always = VK_COMPARE_OP_ALWAYS
		};

		enum StencilOperation 
		{
			SO_Keep = VK_STENCIL_OP_KEEP,
			SO_Zero = VK_STENCIL_OP_ZERO,
			SO_Replace = VK_STENCIL_OP_REPLACE,
			SO_IncrementClamp = VK_STENCIL_OP_INCREMENT_AND_CLAMP,
			SO_DecrementClamp = VK_STENCIL_OP_DECREMENT_AND_CLAMP,
			SO_Invert = VK_STENCIL_OP_INVERT,
			SO_IncrementWrap = VK_STENCIL_OP_INCREMENT_AND_WRAP,
			SO_DecrementWrap = VK_STENCIL_OP_DECREMENT_AND_WRAP
		};

		enum BlendOperation 
		{
			BO_Add = VK_BLEND_OP_ADD,
			BO_Subtract = VK_BLEND_OP_SUBTRACT,
			BO_ReverseSubtract = VK_BLEND_OP_REVERSE_SUBTRACT,
			BO_Min = VK_BLEND_OP_MIN,
			BO_Max = VK_BLEND_OP_MAX
		};

		enum BlendFactor 
		{
			BF_Zero = VK_BLEND_FACTOR_ZERO,
			BF_One = VK_BLEND_FACTOR_ONE,
			BF_SrcColor = VK_BLEND_FACTOR_SRC_COLOR,
			BF_OneMinusSrcColor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
			BF_SrcAlpha = VK_BLEND_FACTOR_SRC_ALPHA,
			BF_OneMinusSrcAlpha = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			BF_DstColor = VK_BLEND_FACTOR_DST_COLOR,
			BF_OneMinusDstColor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
			BF_DstAlpha = VK_BLEND_FACTOR_DST_ALPHA,
			BF_OneMinusDstAlpha = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
			BF_SrcAlphaSaturated = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
			BF_BlendColor = VK_BLEND_FACTOR_CONSTANT_COLOR,
			BF_OneMinusBlendColor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
			BF_BlendAlpha = VK_BLEND_FACTOR_CONSTANT_ALPHA,
			BF_OneMinusBlendAlpha = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA
		};

		enum ColorWriteMask 
		{
			CWM_None = 0,
			CWM_Red = VK_COLOR_COMPONENT_R_BIT,
			CWM_Green = VK_COLOR_COMPONENT_G_BIT,
			CWM_Blue = VK_COLOR_COMPONENT_B_BIT,
			CWM_Alpha = VK_COLOR_COMPONENT_A_BIT,
			CWM_All = CWM_Red | CWM_Green | CWM_Blue
		};

		enum CullMode 
		{
			CM_None = VK_CULL_MODE_NONE,
			CM_Front = VK_CULL_MODE_FRONT_BIT,
			CM_Back = VK_CULL_MODE_BACK_BIT
		};

		enum FaceWinding 
		{
			FW_Clockwise = VK_FRONT_FACE_CLOCKWISE,
			FW_CounterClockwise = VK_FRONT_FACE_COUNTER_CLOCKWISE
		};

		enum TriangleFillMode 
		{
			Fill = VK_POLYGON_MODE_FILL,
			Lines = VK_POLYGON_MODE_LINE
		};

		enum ClipMode 
		{
			CM_Clip = 0,
			CM_Clamp = 1
		};

		enum PrimitiveType 
		{
			PT_Point = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
			PT_Line = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
			PT_LineStrip = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
			PT_Triangle = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			PT_TriangleStrip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP
		};

		enum IndexType 
		{
			IT_Uint16 = VK_INDEX_TYPE_UINT16,
			IT_Uint32 = VK_INDEX_TYPE_UINT32
		};

		enum VisibilityResultMode 
		{
			VRM_Disabled = 0,
			VRM_Boolean = 1,
			VRM_Counting = 2
		};

		enum TextureType 
		{
			TT_Type1D = VK_IMAGE_VIEW_TYPE_1D,
			TT_Type1DArray = VK_IMAGE_VIEW_TYPE_1D_ARRAY,
			TT_Type2D = VK_IMAGE_VIEW_TYPE_2D,
			TT_Type2DArray = VK_IMAGE_VIEW_TYPE_2D_ARRAY,
			TT_Type2DMS = VK_IMAGE_VIEW_TYPE_2D,
			TT_TypeCube = VK_IMAGE_VIEW_TYPE_CUBE,
			TT_Type3D = VK_IMAGE_VIEW_TYPE_3D
		};

		enum TextureUsage 
		{
			TU_Unknown = 0,
			TU_ShaderRead = VK_ACCESS_SHADER_READ_BIT,
			TU_ShaderWrite = VK_ACCESS_SHADER_WRITE_BIT,
			TU_RenderTarget = 2,
			TU_PixelFormatView = 4,
		};

		enum SamplerAddressMode 
		{
			SAM_ClampToEdge = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
			SAM_Repeat = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			SAM_MirrorRepeat = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
			SAM_ClampToZero = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
		};

		enum SamplerMinMagFilter 
		{
			SMMF_Nearest = VK_FILTER_NEAREST,
			SMMF_Linear = VK_FILTER_LINEAR
		};

		enum SamplerMipFilter 
		{
			SMF_NotMipmapped = VK_SAMPLER_MIPMAP_MODE_NEAREST,
			SMF_Nearest = VK_SAMPLER_MIPMAP_MODE_NEAREST,
			SMF_Linear = VK_SAMPLER_MIPMAP_MODE_LINEAR
		};

		enum PixelFormat 
		{
			PF_Invalid = VK_FORMAT_UNDEFINED,
			PF_A8Unorm = VK_FORMAT_R8_UNORM,
			PF_R8Unorm = VK_FORMAT_R8_UNORM,
			PF_R8Snorm = VK_FORMAT_R8_SNORM,
			PF_R8Uint = VK_FORMAT_R8_UINT,
			PF_R8Sint = VK_FORMAT_R8_SINT,
			PF_R16Unorm = VK_FORMAT_R16_UNORM,
			PF_R16Snorm = VK_FORMAT_R16_SNORM,
			PF_R16Uint = VK_FORMAT_R16_UINT,
			PF_R16Sint = VK_FORMAT_R16_SINT,
			PF_R16Float = VK_FORMAT_R16_SFLOAT,
			PF_RG8Unorm = VK_FORMAT_R8G8_UNORM,
			PF_RG8Uint = VK_FORMAT_R8G8_UINT,
			PF_RG8Sint = VK_FORMAT_R8G8_SINT,
			PF_R32Uint = VK_FORMAT_R32_UINT,
			PF_R32Sint = VK_FORMAT_R32_SINT,
			PF_R32Float = VK_FORMAT_R32_SFLOAT,
			PF_RG16Unorm = VK_FORMAT_R16G16_UNORM,
			PF_RG16Snorm = VK_FORMAT_R16G16_SNORM,
			PF_RG16Uint = VK_FORMAT_R16G16_UINT,
			PF_RG16Sint = VK_FORMAT_R16G16_SINT,
			PF_RG16Float = VK_FORMAT_R16G16_SFLOAT,
			PF_RGBA8Unorm = VK_FORMAT_R8G8B8A8_UNORM,
			PF_RGBA8Unorm_sRGB = VK_FORMAT_R8G8B8A8_SRGB,
			PF_RGBA8Snorm = VK_FORMAT_R8G8B8A8_SNORM,
			PF_RGBA8Uint = VK_FORMAT_R8G8B8A8_UINT,
			PF_RGBA8Sint = VK_FORMAT_R8G8B8A8_SINT,
			PF_BGRA8Unorm = VK_FORMAT_B8G8R8A8_UNORM,
			PF_BGRA8Unorm_sRGB = VK_FORMAT_B8G8R8A8_SRGB,
			PF_RGB10A2Unorm = VK_FORMAT_A2R10G10B10_UNORM_PACK32,
			PF_RGB10A2Uint = VK_FORMAT_A2R10G10B10_UINT_PACK32,
			PF_RG11B10Float = VK_FORMAT_B10G11R11_UFLOAT_PACK32,
			PF_RGB9E5Float = VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
			PF_RG32Uint = VK_FORMAT_R32G32_UINT,
			PF_RG32Sint = VK_FORMAT_R32G32_SINT,
			PF_RG32Float = VK_FORMAT_R32G32_SFLOAT,
			PF_RGBA16Unorm = VK_FORMAT_R16G16B16A16_UNORM,
			PF_RGBA16Snorm = VK_FORMAT_R16G16B16A16_SNORM,
			PF_RGBA16Uint = VK_FORMAT_R16G16B16A16_UINT,
			PF_RGBA16Sint = VK_FORMAT_R16G16B16A16_SINT,
			PF_RGBA16Float = VK_FORMAT_R16G16B16A16_SFLOAT,
			PF_RGBA32Uint = VK_FORMAT_R32G32B32A32_UINT,
			PF_RGBA32Sint = VK_FORMAT_R32G32B32A32_SINT,
			PF_RGBA32Float = VK_FORMAT_R32G32B32A32_SFLOAT,
			PF_Depth32Float = VK_FORMAT_D32_SFLOAT,
			PF_Stencil8 = VK_FORMAT_S8_UINT,
			PF_Depth32Float_Stencil8 = VK_FORMAT_D32_SFLOAT_S8_UINT,
		};
	}


	using BufferUsage = RendererEnumsVulkan::BufferUsage;
	using Target = RendererEnumsVulkan::BufferTarget;
	using VertexElementType = RendererEnumsVulkan::VertexElementType;

	using CompareFunction = RendererEnumsVulkan::CompareFunction;
	using StencilOperation = RendererEnumsVulkan::StencilOperation;
	using BlendOperation = RendererEnumsVulkan::BlendOperation;
	using BlendFactor = RendererEnumsVulkan::BlendFactor;
	using CullMode = RendererEnumsVulkan::CullMode;
	using FaceWinding = RendererEnumsVulkan::FaceWinding;
	using TriangleFillMode = RendererEnumsVulkan::TriangleFillMode;
	using ClipMode = RendererEnumsVulkan::ClipMode;
	using PrimitiveType_temp = RendererEnumsVulkan::PrimitiveType;
	using IndexType = RendererEnumsVulkan::IndexType;
	using VisibilityResultMode = RendererEnumsVulkan::VisibilityResultMode;
	using PixelFormat = RendererEnumsVulkan::PixelFormat;
	using TextureType = RendererEnumsVulkan::TextureType;
	using TextureUsage = RendererEnumsVulkan::TextureUsage;
	using SamplerAddressMode = RendererEnumsVulkan::SamplerAddressMode;
	using SamplerMinMagFilter = RendererEnumsVulkan::SamplerMinMagFilter;
	using SamplerMipFilter = RendererEnumsVulkan::SamplerMipFilter;
}


#include "VulkanDevice.hpp"
#include "VulkanBuffers.hpp"
#include "VulkanTextures.hpp"
#include "VulkanPipelineStateConfig.hpp"


namespace Ryujin
{
	using GALDevice = VulkanDevice;
	using GALDevicePtr = VulkanDevice*;
	using GALDeviceRef = VulkanDevice&;

#define DECLARE_RESOURCE_TYPES(GpuType, ResourceType) \
	using GAL##ResourceType = GpuType<Vulkan##ResourceType>; \
	using GAL##ResourceType##Ptr = GpuType<Vulkan##ResourceType>*; \
	using GAL##ResourceType##Ref = GpuType<Vulkan##ResourceType>&; 
	
}