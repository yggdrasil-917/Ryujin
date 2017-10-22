

#include "Scene.hpp"
#include "World.hpp"


namespace Ryujin
{
    Scene::Scene(World* world)
    :
    owner(world)
    {
        
    }
    
    Scene::~Scene()
    {
        
    }
    
    Scene::Scene(const Scene& other)
    :
    entityManager(other.entityManager),
    tagManager(other.tagManager),
    transformManager(other.transformManager),
    meshManager(other.meshManager)
    {
        
    }
    
    Scene& Scene::operator=(const Scene& other)
    {
        entityManager = other.entityManager;
        tagManager = other.tagManager;
        transformManager = other.transformManager;
        meshManager = other.meshManager;
        return *this;
    }
    
    void Scene::LoadScene(BinaryFileReader& blob)
    {
        SceneHeader sceneHeader;
        blob.Next<SceneHeader>(&sceneHeader, 1);
        
        LoadEntities(sceneHeader.numEntities);
        
        LoadTagComponents(blob, sceneHeader);
        
        LoadTransformComponents(blob, sceneHeader);
        
        LoadMeshComponents(blob, sceneHeader);
    }
    
    void Scene::LoadEntities(const int32 entityCount)
    {
        for (int32 e = 0; e < entityCount; ++e)
        {
            entityManager.Create();
        }
    }
    
    void Scene::LoadTagComponents(BinaryFileReader& blob, const SceneHeader& sceneHeader)
    {
        DynamicArray<String> actorTags(sceneHeader.numTags);
        for (uint32 i = 0; i < sceneHeader.numTags; ++i)
        {
            int32 stringBytes;
            blob.Next<int32>(&stringBytes, 1);
            char tag[32] = {0};
            blob.Next(tag, stringBytes);
            actorTags.Add(tag);
        }
        
        // TODO: cooker needs to keep relationship between entity and tag instead of assuming that
        //       every entity will have a tag
        EntityArray entities = entityManager.GetEntityArray();
        if (entities.GetCount() == actorTags.GetSize())
        {
            int32 i = 0;
            for (const auto& e : entities)
            {
                tagManager.SetTag(e.item, actorTags[i++]);
            }
        }
        
    }
    
    void Scene::LoadTransformComponents(BinaryFileReader& blob, const SceneHeader& sceneHeader)
    {
        transformManager.Allocate(sceneHeader.numTransforms);
        DynamicArray<Transform> transforms(sceneHeader.numTransforms);
        Transform* arr = (Transform*)DefaultEngineAllocator::Instance().Allocate(sceneHeader.numTransforms * sizeof(Transform));
        blob.Next(arr, sceneHeader.numTransforms);
        transforms.Copy(arr, sceneHeader.numTransforms);
        DefaultEngineAllocator::Instance().Free(arr);
    }
    
    void Scene::LoadMeshComponents(BinaryFileReader& blob, const SceneHeader& sceneHeader)
    {
        for (uint32 i = 0; i < sceneHeader.numMeshComponents; ++i)
        {
            int32 stringBytes;
            blob.Next<int32>(&stringBytes, 1);
            char meshFile[64] = {0};
            blob.Next(meshFile, stringBytes);
            
//            AssetHandle handle = owner->modelManager.Get(meshFile);
        }
    }
    
    void Scene::UnloadScene()
    {
        tagManager.RemoveAllTags();
    }
    
    void Scene::Update(float dt)
    {
        
    }
    
    void Pup(Pupper* p, Scene& obj, const VarInfo& inVarInfo)
    {
        int32 size = obj.entities.GetSize();
        Pup(p, size, inVarInfo.name + ".length");
        if (p->ioMode == Pupper::IoMode::Read)
        {
            RyujinEntity* entities = RYUJIN_NEW RyujinEntity[size];
            for (int32 i = 0; i < size; ++i)
            {
                Pup(p, entities[i], String::Printf("%s.entity_%d", *inVarInfo.name, i));
            }
            obj.entities.MoveTemp(entities, size);
        }
        else
        {
            int32 i = 0;
            for (auto& e : obj.entities)
            {
                Pup(p, e, String::Printf("%s.entity_%d", *inVarInfo.name, i++));
            }
        }
    }
}
