
#ifndef SCENE_HPP
#define SCENE_HPP


#include "../ECS/ECS.hpp"
#include "RyujinCore/CoreUtils/Json.hpp"
#include "RyujinCore/CoreUtils/BinaryReader.hpp"


#include "RyujinExtensions/EntitySystem/Entity.hpp"
#include "RyujinExtensions/EntitySystem/TransformComponent.hpp"
#include "RyujinExtensions/EntitySystem/MeshComponent.hpp"


namespace Ryujin
{
    struct SceneHeader
    {
        uint32 numEntities;
        uint32 numTags;
        uint32 numTransforms;
        uint32 numMeshComponents;
    };
    
    class Scene
    {
        friend class World;
    private:
        EntityManager entityManager;
        TagManager tagManager;
        TransformComponentManager transformManager;
        MeshComponentManager meshManager;
        
        DynamicArray<RyujinEntity> entities;
        
        class World* owner;
        
        void LoadEntities(const int32 entityCount);
        void LoadTagComponents(BinaryFileReader& blob, const SceneHeader& sceneHeader);
        void LoadTransformComponents(BinaryFileReader& blob, const SceneHeader& sceneHeader);
        void LoadMeshComponents(BinaryFileReader& blob, const SceneHeader& sceneHeader);
        
    public:
        Scene() {}
        Scene(class World* world);
        ~Scene();
        
        Scene(const Scene& other);
        Scene& operator=(const Scene& other);
        
        void LoadScene(BinaryFileReader& blob);
        void UnloadScene();
        
        void Update(float dt);
        
        FORCEINLINE void SetWorld(class World* w) { owner = w; }
        FORCEINLINE class World* GetWorld() const { return owner; }
        
        friend void Pup(Pupper* p, Scene& obj, const VarInfo& inVarInfo);
    };
}



#endif
