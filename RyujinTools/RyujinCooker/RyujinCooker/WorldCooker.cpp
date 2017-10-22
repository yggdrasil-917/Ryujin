

#include "WorldCooker.hpp"
#include "MeshCooker.hpp"
#include "Ryujin/SceneManagement/World.hpp"
#include "Ryujin/Ryujin/Versions.hpp"

#include "RyujinRenderer/Renderer/Model.hpp"
#include "RyujinRenderer/Renderer/SceneView.hpp"
#include "RyujinRenderer/RenderUtils/ImporterWavefront.hpp"



namespace Ryujin
{
    void WorldCooker::CookWorld(const JsonObject& root, MeshCooker& meshCooker, const String& worldFile)
    {
        SCOPED_PROFILE_EVENT(CookingWorld, PC_Cooker, 255, 255, 0);
        bw.Open(String::Printf("%s%s.map", COOKED_CONTENT_DIR, *worldFile), FileMode::FM_Write);
        bw.Write(WORLD_COOKER_VERSION);
        
        const JsonArray& subLevels = root.find("scenes")->second->AsArray();
        
        WorldHeader worldHeader;
        worldHeader.numSublevels = subLevels.GetSize();
        bw.Write(worldHeader);
        
        auto cam = root.find("camera");
        if (cam != root.end())
        {
            const JsonObject& camObject = cam->second->AsObject();
            const JsonArray& lookAtJson = camObject.find("lookat")->second->AsArray();
            const JsonArray& eyeJson = camObject.find("eye")->second->AsArray();
            const JsonArray& upJson = camObject.find("up")->second->AsArray();
            const float nearZ = camObject.find("nearZ")->second->AsNumber();
            const float farZ = camObject.find("farZ")->second->AsNumber();
            const float fov = camObject.find("fov")->second->AsNumber();
            const bool bUsePerspective = camObject.find("UsePerspectiveProjection")->second->AsBool();
            
            Vector3 position(eyeJson[0]->AsNumber(), eyeJson[1]->AsNumber(), eyeJson[2]->AsNumber());
            Vector3 target(lookAtJson[0]->AsNumber(), lookAtJson[1]->AsNumber(), lookAtJson[2]->AsNumber());
            Vector3 up(upJson[0]->AsNumber(), upJson[1]->AsNumber(), upJson[2]->AsNumber());
            SceneView::ProjectionType type = bUsePerspective ? SceneView::ProjectionType::PT_Perspective
                                            : SceneView::ProjectionType::PT_Orthographic;
            
            SceneView view(position, target - position, up, Vector2(nearZ, farZ), fov, type);
            
            bw.Write(view);
        }
        else
        {
            Warnf(Info, "No camera specified! Falling back to default camera");
            
            SceneView view;
            bw.Write(view);
        }
        
        auto skybox = root.find("skybox");
        if (skybox != root.end())
        {
            String skyboxDir = skybox->second->AsObject().find("textureDir")->second->AsString();
            Debugf(Info, "Loaded skybox string: ", *skyboxDir);
            bw.Write(skyboxDir.Length());
            bw.WriteArray(*skyboxDir, skyboxDir.Length());
        }
        
        for (auto& subLevel : subLevels)
        {
            const char* subLevelFile = subLevel->AsString();
            UniquePtr<JsonValue> subLevelJson(Json::ParseFile(*String::Printf("%s%s/%s", MAP_CONTENT_DIR, *worldFile, subLevelFile)));
            RYUJIN_ASSERT(subLevelJson, "Failed to load sublevel json");
            
            const JsonObject& subLevelRoot = subLevelJson->AsObject();
            
            
            // Compile actor related data for deferred processing
            const auto& actors = subLevelRoot.find("actors")->second->AsArray();
            DynamicArray<String> actorTags(actors.GetSize());
            DynamicArray<Transform> actorTransforms(actors.GetSize());
            DynamicArray<MeshComponentToCook> actorMeshComponents(actors.GetSize());
            for (const auto& actor : actors)
            {
                SCOPED_PROFILE_EVENT(ExtractingJsonActorData, PC_Cooker, 255, 255, 0);
                const JsonObject& actorMap = actor->AsObject();
                
                const auto& name = actorMap.find("name");
                if (name != actorMap.end())
                {
                    actorTags.Add(name->second->AsString());
                }
                
                const auto& transformComponent = actorMap.find("transformComponent");
                if (transformComponent != actorMap.end())
                {
                    const JsonObject& transformObject = transformComponent->second->AsObject();
                    const JsonArray& position = transformObject.find("position")->second->AsArray();
                    const JsonArray& orientation = transformObject.find("orientation")->second->AsArray();
                    const JsonArray& scale = transformObject.find("scale")->second->AsArray();
                    
                    Transform transform(Vector3(position[0]->AsNumber(), position[1]->AsNumber(), position[2]->AsNumber()),
                                        Vector3(scale[0]->AsNumber(), scale[1]->AsNumber(), scale[2]->AsNumber()),
                                        Quaternion(orientation[0]->AsNumber(), orientation[1]->AsNumber(), orientation[2]->AsNumber(), 1));
                    actorTransforms.Add(transform);
                }
                
                // Building list of static meshes to cook
                MeshComponentToCook meshComponentToCook;
                const auto& mesh = actorMap.find("meshComponent")->second->AsObject();
                meshComponentToCook.meshFile = mesh.find("mesh")->second->AsString();
                const auto& materialIt = mesh.find("material");
                if (materialIt != mesh.end())
                {
                    meshComponentToCook.materialFile = materialIt->second->AsString();
                }
                else
                {
                    Warnf(Channel::Info, "Mesh component for ", *meshComponentToCook.meshFile, " does not specify material. Using default.");
                    // TODO: haven't decided what the default material should be
                    meshComponentToCook.materialFile = "default";
                }
                actorMeshComponents.Add(meshComponentToCook);
                
                meshCooker.QueueMeshToCook(meshComponentToCook.meshFile);
            }
            
            // Start cooking the scene data
            {
                SCOPED_PROFILE_EVENT(CookingSubLevel, PC_Cooker, 255, 255, 0);
                
                SceneHeader sceneHeader;
                sceneHeader.numEntities = actors.GetSize();
                sceneHeader.numTags = actorTags.GetSize();
                sceneHeader.numTransforms = actorTransforms.GetSize();
                sceneHeader.numMeshComponents = actorMeshComponents.GetSize();
                bw.Write(sceneHeader);
                
                CookTags(actorTags);
                
                CookTransforms(actorTransforms);
                
                CookMeshComponents(actorMeshComponents);
            }
        }
        
        bw.Close();
    }
    
    
    void WorldCooker::CookTags(const DynamicArray<String>& tags)
    {
        SCOPED_PROFILE_EVENT(CookingSceneTags, PC_Cooker, 255, 255, 0);
        
        for (const auto& tag : tags)
        {
            bw.Write(tag.Length());
            bw.WriteArray(*tag, tag.Length());
        }
    }
    
    void WorldCooker::CookTransforms(const DynamicArray<Transform>& transforms)
    {
        SCOPED_PROFILE_EVENT(CookingSceneTransforms, PC_Cooker, 255, 255, 0);
        
        for (const auto& transform : transforms)
        {
            bw.Write(transform);
        }
    }
    
    void WorldCooker::CookMeshComponents(DynamicArray<MeshComponentToCook>& meshComponents)
    {
        SCOPED_PROFILE_EVENT(CookingSceneMeshComponents, PC_Cooker, 255, 255, 0);
        
        for (auto& mesh : meshComponents)
        {
            if (mesh.meshFile.EndsWith(".obj"))
            {
                mesh.meshFile.Replace(mesh.meshFile.Find(".obj"), 6, ".asset");
            }
            bw.Write(mesh.meshFile.Length());
            bw.WriteArray(*mesh.meshFile, mesh.meshFile.Length());
            
//            bw.Write(mesh.materialFile.Length());
//            bw.WriteArray(*mesh.materialFile, mesh.materialFile.Length());
        }
    }
}






