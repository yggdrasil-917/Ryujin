#pragma once


#include "RendererBase.hpp"

#if PLATFORM_MAC
#include "../Metal/MetalBuffer.hpp"
#else
#error "Rendering platform not supported"
#endif

namespace Ryujin
{
#if PLATFORM_MAC
#define _VertexBuffer MetalVertexBuffer
#define _IndexBuffer MetalIndexBuffer
#define _ConstantBuffer MetalConstantBuffer
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
