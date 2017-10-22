#include "MetalBuffer.hpp"
#include "MetalDevice.hpp"


namespace Ryujin
{
    void MetalBuffer::Create(const void* data, uint32 elementSize, uint32 count)
    {
        id<MTLDevice> device = MetalDevice::Instance()->GetMetalDevice();
        bytes = elementSize * count;
        handle = [device newBufferWithBytes:data length:bytes options:MTLResourceOptionCPUCacheModeDefault];
        elementCount = count;
    }
    
    void MetalBuffer::Allocate(uint32 elementSize, uint32 count)
    {
        id<MTLDevice> device = MetalDevice::Instance()->GetMetalDevice();
        bytes = elementSize * count;
        handle = [device newBufferWithLength:bytes options:MTLResourceOptionCPUCacheModeDefault];
        elementCount = count;
    }
    
    void MetalVertexBuffer::Create(const void* data, uint32 elementSize, uint32 count)
    {
        id<MTLDevice> device = MetalDevice::Instance()->GetMetalDevice();
        bytes = elementSize * count;
        handle = [device newBufferWithBytes:data length:bytes options:MTLResourceCPUCacheModeDefaultCache];
        vertexCount = count;
    }
    
    void MetalIndexBuffer::Create(const void* data, uint32 elementSize, uint32 count, bool bInIs16Bit)
    {
        id<MTLDevice> device = MetalDevice::Instance()->GetMetalDevice();
        bytes = elementSize * count;
        handle = [device newBufferWithBytes:data length:bytes options:MTLResourceCPUCacheModeDefaultCache];
        indexCount = count;
        bIs16Bit = bInIs16Bit;
    }
    
    void MetalConstantBuffer::Create(const void* data, uint64 size)
    {
        id<MTLDevice> device = MetalDevice::Instance()->GetMetalDevice();
#if DEBUG // with validation layer on (seems to randomly allocate a bigger buffer than needed and fails at a draw call)
        bytes = size*2;
#else
        bytes = size;
#endif
        handle = [device newBufferWithLength:bytes options:MTLResourceCPUCacheModeDefaultCache];
        if (data)
            Memory::MemCpy([handle contents], (void*)data, size);
    }
    
    void MetalConstantBuffer::Update(void * data, uint64 size)
    {
        Memory::MemCpy([handle contents], data, size);
    }
}
