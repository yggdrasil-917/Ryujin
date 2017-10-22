#pragma once


#include "RenderFeature.hpp"


namespace Ryujin
{
    class RENDER_API SkyboxFeature : public RenderFeature
    {
    private:
        VertexBufferPtr boxVb;
        IndexBufferPtr boxIb;
        TextureCubePtr texture;
        ConstantBufferPtr frameCb;
        RenderPipelineStateDescriptor rpd;
        RenderTargetPoolItemDescriptor rtDesc;
        
    public:
        void Initialize(ShaderModulePtr library, Renderer* renderer) OVERRIDE FINAL;
        void Shutdown() OVERRIDE FINAL;
        void Render(CommandQueue* queue, ShaderModulePtr library) OVERRIDE FINAL;
        
        void PreCachePso() OVERRIDE FINAL;
    };
}
