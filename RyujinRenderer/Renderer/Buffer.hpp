#pragma once


#include "RendererBase.hpp"

#if PLATFORM_MAC
#include "../Metal/MetalBuffer.hpp"
#elif PLATFORM_WINDOWS
#include "../Vulkan/VulkanBuffers.hpp"
#else
#error "Rendering platform not supported"
#endif

#include "RyujinCore/CoreUtils/SmartPointer.hpp"

namespace Ryujin
{
#if PLATFORM_MAC
#define _VertexBuffer MetalVertexBuffer
#define _IndexBuffer MetalIndexBuffer
#define _ConstantBuffer MetalConstantBuffer
#elif PLATFORM_WINDOWS
#define _VertexBuffer VulkanVertexBuffer
#define _IndexBuffer VulkanIndexBuffer
#define _ConstantBuffer VulkanConstantBuffer
#endif
    
    class RENDER_API VertexBuffer : public _VertexBuffer
    {
    };
    using VertexBufferPtr = SharedPtrConcurrent<VertexBuffer>;
    
    class RENDER_API IndexBuffer : public _IndexBuffer
    {
    };
    using IndexBufferPtr = SharedPtrConcurrent<IndexBuffer>;
    
    class RENDER_API ConstantBuffer : public _ConstantBuffer
    {
    };
    using ConstantBufferPtr = SharedPtrConcurrent<ConstantBuffer>;
}
