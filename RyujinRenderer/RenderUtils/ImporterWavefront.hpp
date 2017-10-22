#ifndef WAVEFRONT_IMPORTER_HPP
#define WAVEFRONT_IMPORTER_HPP

#include "MeshImporter.hpp"
#include "RyujinCore/Containers/HashTable.hpp"
#include "RyujinCore/CoreUtils/StringHash.hpp"

namespace Ryujin
{
    
    struct ObjMaterial
    {
        float ka[3];
        float kd[3];
        float ks[3];
        float shininess;
        float alpha;
        int alphaMethod;
        char mapKd[128];
        char mapBump[128];
        char name[128];
        
        ObjMaterial() : alpha(1.0f) {}
    };
    
    
	class RENDER_API ImporterWavefront : public MeshImporter
	{
	public:
        bool LoadStaticMesh(const char* filename, DynamicArray<StaticMeshVertex> &outVerts, DynamicArray<uint32> &outIndices, VertexLayout& outVertexLayout, DynamicArray<SubMesh>& outMeshes) OVERRIDE FINAL;

	private:
		void LoadMaterial(const char * materialFile, HashTable<uint32, ObjMaterial>& materialMap);
	};
}


#endif
