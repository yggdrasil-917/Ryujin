#pragma once


#include "TransformComponent.hpp"
#include "RyujinCore/Math/AABB.hpp"
#include "RyujinRenderer/Renderer/RenderResources.hpp"


namespace Ryujin
{
    class EXTENSIONS_API MeshComponent : public Component
    {
        DECLARE_COMPONENT_TYPE(MeshComponent);
    private:
        VertexBufferPtr vb;
        IndexBufferPtr ib;
        AABB aabb;
        TransformComponent transform;
        
    public:
        MeshComponent();
        
        void CreateFromData(const void* vertexData, uint32 vertexCount, const void* indexData, uint32 indexCount); 
        
        
        FORCEINLINE void SetTransform(const Transform& t) { transform  = TransformComponent(t); }
    };
}
