#pragma once


#include "MetalCommandBuffer.hpp"


namespace Ryujin
{
    class RENDER_API MetalCommandQueue
    {
    protected:
        id<MTLCommandQueue> handle;
        
    public:
        VIRTUAL ~MetalCommandQueue() {}
        
        void Create();
        MetalCommandBuffer* NewCommandBuffer();
        
        void Release();
    };
}
