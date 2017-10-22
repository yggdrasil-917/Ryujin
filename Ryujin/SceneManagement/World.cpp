

#include "World.hpp"
#include "../Ryujin/Versions.hpp"
#include "RyujinCore/OS/Application.hpp"
#include "RyujinCore/CoreUtils/BinaryReader.hpp"
#include "RyujinCore/OS/FileSystem.hpp"


namespace Ryujin
{
    World::World()
    :
    bHasInitializedResourceManagers(false)
    {
        
    }
    
    World::~World()
    {
        
    }
    
    
    void World::LoadWorld(const char *filename)
    {
        if (!bHasInitializedResourceManagers)
        {
            modelManager.Initialize(AppInfo::GetCookedContentDir());
        }
        
        String file = String::Printf("%s%s", AppInfo::GetCookedContentDir(), filename);
        RYUJIN_ASSERT(File::Exists(*file), *String::Printf("Map `%s` not found", *file));
        BinaryFileReader br(*file);
        
        int32 versionId;
        br.Next(&versionId, 1);
        RYUJIN_ASSERT(versionId == WORLD_COOKER_VERSION, "Map cooked with different world cooker version");
        
        WorldHeader worldHeader;
        br.Next(&worldHeader, 1);
        loadedScenes.Resize(worldHeader.numSublevels);
        
        br.Next(&camera, 1);
        
        char skyboxDir[RYUJIN_MAX_PATH] = {0};
        int32 skyboxDirBytes;
        br.Next(&skyboxDirBytes, 1);
        RYUJIN_ASSERT(skyboxDirBytes >= 0 && skyboxDirBytes <= RYUJIN_MAX_PATH, "Bad skybox dir bytes detected");
        br.Next(skyboxDir, skyboxDirBytes);
        
        Scene scene(this);
        scene.LoadScene(br);
        loadedScenes.Add(scene);
    }
    
    
    void World::UnloadWorld()
    {
        for (auto& scene : loadedScenes)
        {
            scene.UnloadScene();
        }
        loadedScenes.RemoveAll();
    }
    
    
    void World::Update(float dt)
    {
        for (auto& scene : loadedScenes)
        {
            scene.Update(dt);
        }
    }
    
    void World::SimpleTest()
    {
        Scene scene;
        scene.owner = this;
        RyujinEntity triangle;
        MeshComponent* mesh = RYUJIN_NEW MeshComponent();
        const float buff[] =
        {
            0.0,  0.5, 0, 1,
            1, 0, 0, 1,
            -0.5, -0.5, 0, 1,
            0, 1, 0, 1,
            0.5, -0.5, 0, 1,
            0, 0, 1, 1,
        };
        mesh->CreateFromData(buff, countof(buff), nullptr, 0);
        mesh->SetTransform(Transform(Vector3(2.5,0.5,-1), Vector3(1.50f, 2.0f, 1.0f), Math::EulerToQuaternion(Vector3(45,0,45))));
        triangle.AddComponent(mesh);
        scene.entities.Add(triangle);
    }
    
    void Pup(Pupper* p, World& obj, const VarInfo& inVarInfo)
    {
        int32 size = obj.loadedScenes.GetSize();
        Pup(p, size, inVarInfo.name + ".length");
        if (p->ioMode == Pupper::IoMode::Read)
        {
            Scene* scenes = RYUJIN_NEW Scene[size];
            for (int32 i = 0; i < size; ++i)
            {
                Pup(p, scenes[i], String::Printf("%s.scene_%d", *inVarInfo.name, i));
            }
            obj.loadedScenes.MoveTemp(scenes, size);
        }
        else
        {
            int32 i = 0;
            for (auto& scene : obj.loadedScenes)
            {
                Pup(p, scene, String::Printf("%s.scene_%d", *inVarInfo.name, i++));
            }
        }
        Pup(p, obj.camera, inVarInfo.name + ".camera");
    }
}
