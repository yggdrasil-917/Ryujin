#include "MetalCommandBuffer.hpp"

#include "MetalRendererCommon.hpp"



namespace Ryujin
{
    MetalGfxEncoder* MetalCommandBuffer::NewGfxEncoderFromDescriptor(const RenderPassDescriptor& desc)
    {
        MTLRenderPassDescriptor *renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
        MetalBackBufferImage* backBuffer = dynamic_cast<MetalBackBufferImage*>(desc.colorAttachments[0].texture);
        if (backBuffer)
        {
            renderPassDescriptor.colorAttachments[0].texture = backBuffer->GetHandle().texture;
            renderPassDescriptor.colorAttachments[0].loadAction = gLoadAction[desc.colorAttachments[0].loadAction];
            renderPassDescriptor.colorAttachments[0].storeAction = gStoreAction[desc.colorAttachments[0].storeAction];
            renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(desc.colorAttachments[0].clearColor[0],
                                                                                    desc.colorAttachments[0].clearColor[1],
                                                                                    desc.colorAttachments[0].clearColor[2],
                                                                                    desc.colorAttachments[0].clearColor[3]);
        }
        else
        {
            const int32 maxAttachments = countof(desc.colorAttachments);
            for (int32 i = 0; i < maxAttachments; ++i)
            {
                if (desc.colorAttachments[i].texture == nullptr)
                {
                    break;
                }
                MetalTexture* texture = dynamic_cast<MetalTexture*>(desc.colorAttachments[i].texture);
                renderPassDescriptor.colorAttachments[i].texture = texture->GetHandle();
                renderPassDescriptor.colorAttachments[i].loadAction = gLoadAction[desc.colorAttachments[i].loadAction];
                renderPassDescriptor.colorAttachments[i].storeAction = gStoreAction[desc.colorAttachments[i].storeAction];
                renderPassDescriptor.colorAttachments[i].clearColor = MTLClearColorMake(desc.colorAttachments[i].clearColor[0],
                                                                                        desc.colorAttachments[i].clearColor[1],
                                                                                        desc.colorAttachments[i].clearColor[2],
                                                                                        desc.colorAttachments[i].clearColor[3]);
            }
        }
        
        if (desc.depthAttachment.texture)
        {
            renderPassDescriptor.depthAttachment.texture = ((MetalTexture*)desc.depthAttachment.texture)->GetHandle();
            renderPassDescriptor.depthAttachment.loadAction = gLoadAction[desc.depthAttachment.loadAction];
            renderPassDescriptor.depthAttachment.storeAction = gStoreAction[desc.depthAttachment.storeAction];
            renderPassDescriptor.depthAttachment.clearDepth = desc.depthAttachment.clearColor[0];
            renderPassDescriptor.stencilAttachment.clearStencil = desc.depthAttachment.clearColor[1];
        }
        MetalGfxEncoder* encoder = RYUJIN_NEW MetalGfxEncoder();
        encoder->handle = [handle renderCommandEncoderWithDescriptor:renderPassDescriptor];
        return encoder;
    }
    
    void MetalCommandBuffer::Commit()
    {
        [handle commit];
    }
    
    void MetalCommandBuffer::WaitUntilDone()
    {
        [handle waitUntilCompleted];
    }
    
    void MetalCommandBuffer::Present(MetalBackBufferImage* image)
    {
        [handle presentDrawable:image->GetHandle()];
    }
    
    void MetalCommandBuffer::Release()
    {
        if (handle)
        {
            [handle release];
        }
    }
}
