#pragma once


#include "MeshImporter.hpp"

#include "../Renderer/VertexFormat.hpp"


namespace Ryujin
{
    struct CookedMeshHeader
    {
        VertexLayout vertexLayout;
        int32 meshCookerVersion;
        int32 vertCount;
        int32 indexCount;
        int32 meshCount;
        bool bHas16BitIndices;
    };
    
	class RENDER_API ImporterRmf : public MeshImporter
	{
	public:
        bool LoadStaticMesh(const char* filename, DynamicArray<uint8> &outVerts, DynamicArray<uint32> &outIndices, VertexLayout& outVertexLayout, DynamicArray<SubMesh>& outMeshes) OVERRIDE FINAL;
        
        bool LoadStaticMesh(uint8* binaryBlob, PtrSize byteSize, DynamicArray<uint8> &outVerts, DynamicArray<uint32> &outIndices, VertexLayout& outVertexLayout, DynamicArray<SubMesh>& outMeshes) OVERRIDE FINAL;
	};
}

