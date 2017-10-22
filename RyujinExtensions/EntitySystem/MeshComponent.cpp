#include "MeshComponent.hpp"
#include "RyujinRenderer/Renderer/RendererCommon.hpp"

namespace Ryujin
{
    IMPLEMENT_COMPONENT_TYPE(MeshComponent);
    
    MeshComponent::MeshComponent()
    : Component()
    {
        
    }
    
    void MeshComponent::CreateFromData(const void *vertexData, uint32 vertexCount, const void *indexData, uint32 indexCount)
    {
        RYUJIN_ASSERT(vertexData, "Null vertex data");
        
        vb = GfxDevice::GetDevice()->NewVertexBuffer(vertexData, sizeof(float), vertexCount);
        if(indexData)
        {
            ib = GfxDevice::GetDevice()->NewIndexBuffer(indexData, sizeof(uint16), indexCount, true);
        }
        else
        {
            ib = nullptr;
        }
    }
}
