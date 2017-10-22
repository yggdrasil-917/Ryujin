#include "MetalCommandEncoder.hpp"

#include "MetalRendererCommon.hpp"

namespace Ryujin
{
    void MetalGfxEncoder::PushDebugGroup(const char* name)
    {
        [handle pushDebugGroup:[NSString stringWithUTF8String:name]];
    }
    
    void MetalGfxEncoder::PopDebugGroup()
    {
        [handle popDebugGroup];
    }
    
    void MetalGfxEncoder::SetPipelineState(const MetalPipelineState* pso)
    {
        [handle setRenderPipelineState:pso->GetHandle()];
    }
    
    void MetalGfxEncoder::SetVertexBufferBytes(const void *data, uint64 size, uint32 slot)
    {
        [handle setVertexBytes:data length:size atIndex:slot];
    }
    
    void MetalGfxEncoder::SetVertexBuffer(MetalVertexBuffer* buffer, uint32 offset, uint32 slot)
    {
        [handle setVertexBuffer:buffer->GetHandle() offset:offset atIndex:slot];
    }
    
    void MetalGfxEncoder::SetIndexBuffer(MetalIndexBuffer* buffer, uint32 offset, uint32 slot)
    {
        [handle setVertexBuffer:buffer->GetHandle() offset:offset atIndex:slot];
    }
    
    void MetalGfxEncoder::SetVertexConstantBuffer(MetalConstantBuffer* buffer, uint32 offset, uint32 slot)
    {
        [handle setVertexBuffer:buffer->GetHandle() offset:offset atIndex:slot];
    }
    
    void MetalGfxEncoder::SetFragmentConstantBuffer(MetalConstantBuffer* buffer, uint32 offset, uint32 slot)
    {
        [handle setFragmentBuffer:buffer->GetHandle() offset:offset atIndex:slot];
    }
    
    void MetalGfxEncoder::SetDepthStencilState(CompareFunction compareFunc, bool bDepthWriteEnabled)
    {
        id<MTLDepthStencilState> ds = MetalDevice::Instance()->GetOrCreateDepthStencilState(compareFunc, bDepthWriteEnabled);
        [handle setDepthStencilState:ds];
    }
    
    void MetalGfxEncoder::SetTriangleWinding(FaceWinding winding)
    {
        [handle setFrontFacingWinding:gWinding[winding]];
    }
    
    void MetalGfxEncoder::SetCullingMode(CullMode mode)
    {
        [handle setCullMode:gCullMode[mode]];
    }
    
    void MetalGfxEncoder::SetFragmentTexture(MetalTexture *texture, uint32 slot)
    {
        [handle setFragmentTexture:texture->GetHandle() atIndex:slot];
    }
    
    void MetalGfxEncoder::SetFragmentSampler(MetalSampler *sampler, uint32 slot)
    {
        [handle setFragmentSamplerState:sampler->GetHandle() atIndex:slot];
    }
    
    void MetalGfxEncoder::SetViewport(const Rect<float>& rect, float zNear, float zFar)
    {
        MTLViewport viewport;
        viewport.originX = rect.x;
        viewport.originY = rect.y;
        viewport.width = rect.width;
        viewport.height = rect.height;
        viewport.znear = zNear;
        viewport.zfar = zFar;
        [handle setViewport:viewport];
    }
    
    void MetalGfxEncoder::SetScissorRect(const Rect<uint16>& rect)
    {
        MTLScissorRect mtlRect;
        mtlRect.x = rect.x;
        mtlRect.y = rect.y;
        mtlRect.width = rect.width;
        mtlRect.height = rect.height;
        [handle setScissorRect:mtlRect];
    }
    
    void MetalGfxEncoder::Draw(uint32 vertexCount, uint32 vertexStart)
    {
        [handle drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:vertexStart vertexCount:vertexCount instanceCount:1];
    }
    
    void MetalGfxEncoder::DrawIndexed(MetalIndexBuffer* ib)
    {
        [handle
         drawIndexedPrimitives:MTLPrimitiveTypeTriangle
         indexCount:ib->GetIndexCount()
         indexType:(ib->Is16Bit() ? MTLIndexTypeUInt16 : MTLIndexTypeUInt32)
         indexBuffer:ib->GetHandle()
         indexBufferOffset:0];
    }
    
    void MetalGfxEncoder::Finish()
    {
        [handle endEncoding];
    }
    
    void MetalGfxEncoder::Release()
    {
        if (handle)
        {
            [handle release];
        }
    }
    
}
