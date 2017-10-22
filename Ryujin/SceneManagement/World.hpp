
#ifndef WORLD_HPP
#define WORLD_HPP


#include "Scene.hpp"
#include "../ResourceManagers/TextureResourceManager.hpp"
#include "../ResourceManagers/ModelResourceManager.hpp"

#include "RyujinRenderer/Renderer/SceneView.hpp"


namespace Ryujin
{
    struct WorldHeader
    {
        int32 numSublevels;
    };
    
    
    class World
    {
        friend class Scene;
        
    private:
        DynamicArray<Scene> loadedScenes;
        TextureManager textureManager;
        ModelManager modelManager;
        SceneView camera;
        bool bHasInitializedResourceManagers;
        
    public:
        World();
        ~World();
        
        void LoadWorld(const char* filename);
        void UnloadWorld();
        void Update(float dt);
        
        void SimpleTest();
        
        friend void Pup(Pupper* p, World& obj, const VarInfo& inVarInfo);
    };
}


#endif
