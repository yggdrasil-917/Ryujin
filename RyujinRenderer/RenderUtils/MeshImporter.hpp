#ifndef MESH_IMPORTER_HPP
#define MESH_IMPORTER_HPP


#include "../Renderer/Model.hpp"
#include "RyujinCore/Logger/Logger.hpp"
#include "RyujinCore/CoreUtils/BinaryReader.hpp"
#include "RyujinCore/Containers/DynamicArray.hpp"


#define WITH_STBI 1

namespace Ryujin
{

	class RENDER_API MeshImporter
	{
	public:
        VIRTUAL bool LoadStaticMesh(const char* filename, DynamicArray<StaticMeshVertex> &outVerts, DynamicArray<uint32> &outIndices, VertexLayout& outVertexLayout, DynamicArray<SubMesh>& outMeshes) { return false; }
        
        VIRTUAL bool LoadStaticMesh(const char* filename, DynamicArray<uint8> &outVerts, DynamicArray<uint32> &outIndices, VertexLayout& outVertexLayout, DynamicArray<SubMesh>& outMeshes) { return false; }
        
        VIRTUAL bool LoadStaticMesh(uint8* binaryBlob, PtrSize byteSize, DynamicArray<uint8> &outVerts, DynamicArray<uint32> &outIndices, VertexLayout& outVertexLayout, DynamicArray<SubMesh>& outMeshes) { return false; }
	};
}


#endif
