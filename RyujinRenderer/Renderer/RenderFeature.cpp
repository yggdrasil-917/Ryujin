#include "RenderFeature.hpp"
#include "RyujinCore/CoreUtils/CRC.hpp"


namespace Ryujin
{
    void RenderFeature::Shutdown()
    {
        HashTable<uint64, PipelineStatePtr>::Iterator it(cachedPsoMap);
        while (it)
        {
            it->value.SafeRelease();
            ++it;
        }
    }
    
    PipelineStatePtr RenderFeature::GetOrCachePso(ShaderModulePtr shaderLibrary, RenderPipelineStateDescriptor& rpd)
    {
        PipelineStatePtr pso(nullptr);
        uint64 hash = CRC::MemCrc(reinterpret_cast<uint8*>(&rpd), sizeof(rpd));
        if (cachedPsoMap.TryGetValue(hash, pso))
        {
            return pso;
        }
        
        pso = PipelineStatePtr(GfxDevice::GetDevice()->NewPipelineState(shaderLibrary, rpd));
        cachedPsoMap.Add(hash, pso);
        
        return pso;
    }
}
