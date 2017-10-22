#include "MetalCommandQueue.hpp"
#include "MetalDevice.hpp"

#include "MetalSwapChain.hpp"
#include "MetalPipelineState.hpp"
#include "MetalBuffer.hpp"

#include "../Renderer/RendererCommon.hpp"

namespace Ryujin
{
    void MetalCommandQueue::Create()
    {
        id<MTLDevice> device = MetalDevice::Instance()->GetMetalDevice();
        handle = [device newCommandQueue];
    }
    
    MetalCommandBuffer* MetalCommandQueue::NewCommandBuffer()
    {
        MetalCommandBuffer* buffer = RYUJIN_NEW MetalCommandBuffer();
        buffer->handle = [handle commandBufferWithUnretainedReferences];
        return buffer;
    }
    
    void MetalCommandQueue::Release()
    {
        if (handle)
        {
            [handle release];
        }
    }
}
