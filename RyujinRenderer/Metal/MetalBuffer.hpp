
#pragma once

#include "../Common/BufferBase.hpp"

#if PLATFORM_MAC

#import <Metal/Metal.h>

namespace Ryujin
{
    class RENDER_API MetalBuffer : public BufferBase
    {
    public:
        id<MTLBuffer> handle;
        id<MTLBuffer> apiBufferHandle;
        
        
        MetalBuffer(id<MTLBuffer> inAPIBufferHandle)
        : apiBufferHandle(inAPIBufferHandle)
        {
            
        }
        
        VIRTUAL ~MetalBuffer()
        {
            if (apiBufferHandle != nil)
            {
                [apiBufferHandle setPurgeableState:MTLPurgeableStateEmpty]; // This may need to be added when the buffer is created instead
                //[apiBufferHandle release];
                
                apiBufferHandle = nil;
            }
        }
        
        void Create(const void* data, uint32 elementSize, uint32 count);
        void Allocate(uint32 elementSize, uint32 count);
        
        FORCEINLINE id<MTLBuffer> GetHandle() const { return handle; }
    };
    
    class RENDER_API MetalVertexBuffer : public VertexBufferBase
    {
    protected:
        id<MTLBuffer> handle;
        
    public:
        VIRTUAL ~MetalVertexBuffer() {}
        void Create(const void* data, uint32 elementSize, uint32 count);
        
        FORCEINLINE id<MTLBuffer> GetHandle() const { return handle; }
    };
    
    class RENDER_API MetalIndexBuffer : public IndexBufferBase
    {
    protected:
        id<MTLBuffer> handle;
        
    public:
        VIRTUAL ~MetalIndexBuffer() {}
        void Create(const void* data, uint32 elementSize, uint32 count, bool bIs16Bit);
        
        FORCEINLINE id<MTLBuffer> GetHandle() const { return handle; }
    };
    
    class RENDER_API MetalConstantBuffer : public ConstantBufferBase
    {
    protected:
        id<MTLBuffer> handle;
        
    public:
        VIRTUAL ~MetalConstantBuffer() {}
        void Create(const void* data, uint64 size);
        void Update(void* data, uint64 size);
        
        FORCEINLINE id<MTLBuffer> GetHandle() const { return handle; }
    };
}

#endif // PLATFORM_MAC

