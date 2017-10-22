#pragma once


#include "RyujinCore/OS/Platform.hpp"
#include "RyujinCore/Math/Math.hpp"

#include "RendererConfig.hpp"




#if PLATFORM_WINDOWS

#if RYUJIN_RENDERER_EXPORT
#define RENDER_API __declspec(dllexport)
#else
#define RENDER_API __declspec(dllimport)
#endif

#elif PLATFORM_MAC
#define RENDER_API __attribute__ ((visibility ("default")))

#else
#define RENDER_API __attribute__ ((visibility ("default")))

#endif



//#define USE_64_BIT_HANDLES 1

#if USE_64_BIT_HANDLES
#define VERTEX_BUFFER_INDEX_BITS   32
#define VERTEX_BUFFER_GEN_BITS     32

#define INDEX_BUFFER_INDEX_BITS   32
#define INDEX_BUFFER_GEN_BITS     32

#define CONSTANT_BUFFER_INDEX_BITS   32
#define CONSTANT_BUFFER_GEN_BITS     32

#define DEFAULT_BUFFER_INDEX_BITS   32
#define DEFAULT_BUFFER_GEN_BITS     32

#define DEFAULT_RESOURCE_INDEX_BITS 48
#define DEFAULT_RESOURCE_GEN_BITS   16

#else
#define VERTEX_BUFFER_INDEX_BITS   12
#define VERTEX_BUFFER_GEN_BITS     20

#define INDEX_BUFFER_INDEX_BITS   12
#define INDEX_BUFFER_GEN_BITS     20

#define CONSTANT_BUFFER_INDEX_BITS   16
#define CONSTANT_BUFFER_GEN_BITS     16

#define DEFAULT_BUFFER_INDEX_BITS   16
#define DEFAULT_BUFFER_GEN_BITS     16

#define DEFAULT_RESOURCE_INDEX_BITS 24
#define DEFAULT_RESOURCE_GEN_BITS   8

#endif


namespace Ryujin
{
    using BufferHandle = Handle<DEFAULT_BUFFER_INDEX_BITS, DEFAULT_BUFFER_GEN_BITS>;
    using VertexBufferHandle = Ryujin::Handle<VERTEX_BUFFER_INDEX_BITS, VERTEX_BUFFER_GEN_BITS>;
    using IndexBufferHandle = Ryujin::Handle<INDEX_BUFFER_INDEX_BITS, INDEX_BUFFER_GEN_BITS>;
    using ConstantBufferHandle = Ryujin::Handle<CONSTANT_BUFFER_INDEX_BITS, CONSTANT_BUFFER_GEN_BITS>;
    using ShaderHandle = Ryujin::Handle<DEFAULT_RESOURCE_INDEX_BITS, DEFAULT_RESOURCE_GEN_BITS>;
    using TextureHandle = Ryujin::Handle<DEFAULT_RESOURCE_INDEX_BITS, DEFAULT_RESOURCE_GEN_BITS>;
    using MaterialHandle = Ryujin::Handle<DEFAULT_RESOURCE_INDEX_BITS, DEFAULT_RESOURCE_GEN_BITS>;
    using ModelHandle = Ryujin::Handle<DEFAULT_RESOURCE_INDEX_BITS, DEFAULT_RESOURCE_GEN_BITS>;
}

