
#ifndef WORLD_COOKER_HPP
#define WORLD_COOKER_HPP


#include "CookerCommon.hpp"

#include "RyujinCore/Math/Transform.hpp"
#include "RyujinCore/Containers/String.hpp"
#include "RyujinCore/CoreUtils/BinaryWriter.hpp"


namespace Ryujin
{
    class StaticMesh;
    
    struct MeshComponentToCook
    {
        String meshFile;
        String materialFile;
    };
    
    class WorldCooker
    {
    private:
        BinaryWriter bw;
        
        void CookTags(const DynamicArray<String>& tags);
        void CookTransforms(const DynamicArray<Transform>& transforms);
        void CookMeshComponents(DynamicArray<MeshComponentToCook>& meshComponents);
        
    public:
        void CookWorld(const JsonObject& root, class MeshCooker& meshCooker, const String& worldFile);
    };
}



#endif