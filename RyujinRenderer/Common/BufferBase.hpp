#pragma once


#include "../Renderer/RendererBase.hpp"

namespace Ryujin
{
    class RENDER_API BufferBase
    {
    protected:
        uint32 elementCount;
        uint64 bytes;
        
    public:
        FORCEINLINE uint32 GetCount() const { return elementCount; }
        FORCEINLINE uint64 GetByteSize() const { return bytes; }
    };
    
    class RENDER_API VertexBufferBase
    {
    protected:
        uint32 vertexCount;
        uint64 bytes;
        
    public:
        
        FORCEINLINE uint32 GetVertexCount() const { return vertexCount; }
    };
    
    
    class RENDER_API IndexBufferBase
    {
    protected:
        uint32 indexCount;
        uint64 bytes;
        bool bIs16Bit;
        
    public:
        
        FORCEINLINE uint32 GetIndexCount() const { return indexCount; }
        FORCEINLINE bool Is16Bit() const { return bIs16Bit; }
    };
    
    
    class RENDER_API ConstantBufferBase
    {
    protected:
        uint64 bytes;
        
    public:
        FORCEINLINE uint64 GetByteSize() const { return bytes; }
    };
}
