
#ifndef MESH_COOKER_HPP
#define MESH_COOKER_HPP


#include "CookerCommon.hpp"

#include "RyujinRenderer/RenderUtils/ImporterWavefront.hpp"



namespace Ryujin
{
    class StaticMesh;
    
    
    class MeshCooker
    {
    private:
        DynamicArray<CookableItem> meshesToCook;
        
    public:
        void QueueMeshToCook(const String& meshSourceFile);
        void RemoveAllMeshes();
        
        void CookQueuedMeshes();
        void CookMesh(const CookableItem& mesh, bool bInterleaveVertexData);
    };
}



#endif
