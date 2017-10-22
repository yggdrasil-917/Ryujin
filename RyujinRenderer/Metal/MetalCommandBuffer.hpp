#pragma once


#include "MetalCommandEncoder.hpp"


namespace Ryujin
{
    class RENDER_API MetalCommandBuffer
    {
        friend class MetalCommandQueue;
        
    protected:
        id<MTLCommandBuffer> handle;
        
    public:
        MetalCommandBuffer() {}
        VIRTUAL ~MetalCommandBuffer() {}
        
        void Commit();
        void Present(class MetalBackBufferImage* image);
        MetalGfxEncoder* NewGfxEncoderFromDescriptor(const struct RenderPassDescriptor& desc);
        void WaitUntilDone();
        
        void Release();
        
        FORCEINLINE id<MTLCommandBuffer> GetHandle() const { return handle; }
    };
}
