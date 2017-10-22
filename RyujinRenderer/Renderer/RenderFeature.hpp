#pragma once


#include "RendererCommon.hpp"
#include "RenderResources.hpp"
#include "RenderTargetPool.hpp"


namespace Ryujin
{
    class RENDER_API RenderFeature
    {
    protected:
        HashTable<uint64, PipelineStatePtr> cachedPsoMap;
        
    public:
        VIRTUAL void Initialize(ShaderModulePtr library, class Renderer* renderer) {}
        VIRTUAL void Shutdown();
        VIRTUAL void Render(CommandQueue* queue, ShaderModulePtr library) {}
        
        PipelineStatePtr GetOrCachePso(ShaderModulePtr shaderLibrary, RenderPipelineStateDescriptor& rpd);
        VIRTUAL void PreCachePso() = 0;
    };
}
