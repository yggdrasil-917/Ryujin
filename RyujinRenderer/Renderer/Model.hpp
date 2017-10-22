
#ifndef MODEL_HPP
#define MODEL_HPP


#include "RyujinCore/Containers/DynamicArray.hpp"
#include "RyujinCore/CoreUtils/SmartPointer.hpp"
#include "RyujinCore/Math/AABB.hpp"
#include "RenderResources.hpp"
#include "VertexFormat.hpp"


namespace Ryujin
{
    struct SubMesh
    {
        MaterialHandle material;
        uint32 startIndex;
        uint32 endIndex;
        uint32 elementCount;
        uint32 indexOffset;
    };
    
    
    class ModelResource
    {
    private:
        UniquePtr<GALBuffer> vertexBuffer;
        UniquePtr<GALBuffer> indexBuffer;
        DynamicArray<SubMesh> mesh;
        
    public:
        ModelResource(UniquePtr<GALBuffer>&& inVertexBuffer, UniquePtr<GALBuffer>&& inIndexBuffer, DynamicArray<SubMesh>&& inSubmesh)
        :   mesh(Move(inSubmesh))
        {
//            vertexBuffer = Move(inVertexBuffer);
//            indexBuffer = Move(inIndexBuffer);
        }
        
        FORCEINLINE UniquePtr<GALBuffer>& GetVertexBuffer() { return vertexBuffer; }
        FORCEINLINE UniquePtr<GALBuffer>& GetIndexBuffer() { return indexBuffer; }
        FORCEINLINE const DynamicArray<SubMesh>& GetMeshes() const { return mesh; }
    };
    
    
    struct StaticMeshVertex
    {
        Vector3 position;
        Vector2 uv;
        Vector3 normal;
        Vector3 tangent;
        Vector2 uv2;
        Vector4 color;
    };
    
    class StaticMesh
    {
    public:
        DynamicArray<StaticMeshVertex> verts;
        DynamicArray<uint32> indices;
        AABB boundingBox;

        DynamicArray<SubMesh> meshes;
        
        StaticMesh() {}
        ~StaticMesh()
        {
            
        }
        
        FORCEINLINE void SetVertexData(StaticMeshVertex* inVertData, uint32 inVertCount)
        {
            verts.Copy(inVertData, inVertCount);
        }
        
        FORCEINLINE void SetIndexData(uint32* inIndexData, uint32 inIndexCount)
        {
            indices.Copy(inIndexData, inIndexCount);
        }
        
        FORCEINLINE void SetSubmeshData(SubMesh* inSubmesh, uint32 inSubmeshCount)
        {
            meshes.Copy(inSubmesh, inSubmeshCount);
        }
    };
    
    struct StaticMesh2
    {
        byte* vertexBlob;
        VertexLayout vertexLayout;
        DynamicArray<uint32> indices;
        DynamicArray<SubMesh> meshes;
        AABB boundingBox;
    };
}


#endif
