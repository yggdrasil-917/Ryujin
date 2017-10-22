
#ifndef MODEL_RESOURCE_MANAGER_HPP
#define MODEL_RESOURCE_MANAGER_HPP


#include "ResourceManager.hpp"
#include "../Ryujin/Versions.hpp"
#include "RyujinRenderer/Renderer/Model.hpp"
#include "RyujinRenderer/Renderer/RenderResourceFactory.hpp"
#include "RyujinCore/CoreUtils/BinaryReader.hpp"
#include "RyujinCore/CoreUtils/ServiceLocator.hpp"


namespace Ryujin
{
    class ModelManager : public ResourceManager<ModelResource>
    {
    protected:
        
        void LoadDefaultAsset() OVERRIDE FINAL
        {
            defaultHandle = Get("cube.asset");
        }
        
        
        UniquePtr<ModelResource> Load(const String& filename) OVERRIDE FINAL
        {
            UniquePtr<MeshResource> meshResource(MeshResourceLoader::Load(filename));
            
            BufferDescriptor vertBufferDesc;
            vertBufferDesc.bytesToAllocate = sizeof(StaticMeshVertex) * meshResource->GetVerts().GetSize();
            vertBufferDesc.data = (uint8*)meshResource->GetVerts().GetData();
            vertBufferDesc.bufferUsage = BufferUsage::BU_StaticDraw;
//            vertBufferDesc.target = Target::BT_VertexBuffer;
            UniquePtr<GALBuffer> vertexBuffer = RenderResourceFactory::CreateBuffer(vertBufferDesc);
            
            BufferDescriptor indexBufferDesc;
            indexBufferDesc.bytesToAllocate = sizeof(uint32) * meshResource->GetIndices().GetSize();
            indexBufferDesc.data = (uint8*)meshResource->GetIndices().GetData();
            indexBufferDesc.bufferUsage = BufferUsage::BU_StaticDraw;
//            indexBufferDesc.target = Target::BT_IndexBuffer;
            UniquePtr<GALBuffer> indexBuffer = RenderResourceFactory::CreateBuffer(indexBufferDesc);
            DynamicArray<SubMesh> mesh = meshResource->GetMeshes();
            
            // Need to add a material manager
            
            return UniquePtr<ModelResource>(RYUJIN_NEW ModelResource(Move(vertexBuffer), Move(indexBuffer), Move(mesh)));
        }
        
        void Unload(AssetHandle inHandle) OVERRIDE FINAL
        {
            if (!resources.Contains(inHandle.handle))
                return;
            
            ModelResource* resource = resources.GetObject(inHandle.handle).Get();
            RenderResourceFactory::DestroyBuffer(resource->GetVertexBuffer());
            RenderResourceFactory::DestroyBuffer(resource->GetIndexBuffer());
            SAFE_DELETE(resource);
            resources.Remove(inHandle.handle);
        }
    };
}


#endif
