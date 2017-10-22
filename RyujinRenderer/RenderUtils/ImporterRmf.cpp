
#include "ImporterRmf.hpp"

#include "RyujinCore/Containers/String.hpp"
#include "RyujinCore/Containers/DynamicArray.hpp"

#include "RyujinCore/OS/FileSystem.hpp"


namespace Ryujin
{
    bool ImporterRmf::LoadStaticMesh(const char *filename, DynamicArray<uint8> &outVerts, DynamicArray<uint32> &outIndices, VertexLayout& outVertexLayout, DynamicArray<SubMesh> &outMeshes)
    {
        BinaryFileReader br(filename);
        if (!br.IsOpen())
            return false;
        
//        RYUJIN_ASSERT(versionId == MESH_COOKER_VERSION, "Mesh was cooked with different cooker version");
        
        CookedMeshHeader header;
        br.Next(&header, 1);
        outVertexLayout = header.vertexLayout;
        uint8* verts = RYUJIN_NEW uint8[header.vertCount * outVertexLayout.GetByteSize()];
        uint32* indices = RYUJIN_NEW uint32[header.indexCount];
        SubMesh* meshes = RYUJIN_NEW SubMesh[header.meshCount];
        
        br.Next(verts, header.vertCount * outVertexLayout.GetByteSize());
        br.Next(indices, header.indexCount);
        br.Next(meshes, header.meshCount);
        
        outVerts.MoveTemp(verts, header.vertCount);
        outIndices.MoveTemp(indices, header.indexCount);
        outMeshes.MoveTemp(meshes, header.meshCount);
        
        return true;
    }
    
    
    bool ImporterRmf::LoadStaticMesh(uint8* binaryBlob, PtrSize byteSize, DynamicArray<uint8> &outVerts, DynamicArray<uint32> &outIndices, VertexLayout& outVertexLayout, DynamicArray<SubMesh>& outMeshes)
    {
        BinaryReader br(binaryBlob, byteSize);
        
        CookedMeshHeader header;
        header = br.Next<CookedMeshHeader>();
        outVertexLayout = header.vertexLayout;
        uint8* verts = RYUJIN_NEW uint8[header.vertCount * outVertexLayout.GetByteSize()];
        uint32* indices = RYUJIN_NEW uint32[header.indexCount];
        SubMesh* meshes = RYUJIN_NEW SubMesh[header.meshCount];
        
        verts = br.NextArray<uint8>(header.vertCount * outVertexLayout.GetByteSize());
        indices = br.NextArray<uint32>(header.indexCount);
        meshes = br.NextArray<SubMesh>(header.meshCount);
        
        outVerts.MoveTemp(verts, header.vertCount * outVertexLayout.GetByteSize());
        outIndices.MoveTemp(indices, header.indexCount);
        outMeshes.MoveTemp(meshes, header.meshCount);
        
        return true;
    }
}
