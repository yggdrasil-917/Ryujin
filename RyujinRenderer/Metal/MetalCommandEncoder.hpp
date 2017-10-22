#pragma once



#include "../Renderer/RendererBase.hpp"
#include "../Common/ResourceDescriptors.hpp"
#import <Metal/Metal.h>



namespace Ryujin
{
    class RENDER_API MetalCommandEncoder
    {
        friend class MetalCommandBuffer;
        
    protected:
        id<MTLRenderCommandEncoder> handle;
        
    public:
        VIRTUAL ~MetalCommandEncoder() {}
    };
    
    class RENDER_API MetalGfxEncoder : public MetalCommandEncoder
    {
    public:
        MetalGfxEncoder() {}
        VIRTUAL ~MetalGfxEncoder() {}
        
        void PushDebugGroup(const char* name);
        void PopDebugGroup();
        void SetPipelineState(const class MetalPipelineState* pso);
        void SetVertexBufferBytes(const void* data, uint64 size, uint32 slot);
        void SetVertexBuffer(class MetalVertexBuffer* buffer, uint32 offset, uint32 slot);
        void SetIndexBuffer(class MetalIndexBuffer* buffer, uint32 offset, uint32 slot);
        void SetVertexConstantBuffer(class MetalConstantBuffer* buffer, uint32 offset, uint32 slot);
        void SetFragmentConstantBuffer(class MetalConstantBuffer* buffer, uint32 offset, uint32 slot);
        void SetDepthStencilState(CompareFunction compareFunc, bool bDepthWriteEnabled);
        void SetTriangleWinding(FaceWinding winding);
        void SetCullingMode(CullMode mode);
        void SetFragmentTexture(class MetalTexture* texture, uint32 slot);
        void SetFragmentSampler(class MetalSampler* sampler, uint32 slot);
        void SetViewport(const Rect<float>& rect, float zNear = 0.0f, float zFar = 1.0f);
        void SetScissorRect(const Rect<uint16>& rect);
        void Draw(uint32 vertexCount, uint32 vertexStart = 0);
        void DrawIndexed(MetalIndexBuffer* ib);
        void Finish();
        
        void Release();
        
        FORCEINLINE id<MTLRenderCommandEncoder> GetHandle() const { return handle; }
    };
    
    class RENDER_API MetalComputeEncoder : public MetalCommandEncoder
    {
        
    };
}
