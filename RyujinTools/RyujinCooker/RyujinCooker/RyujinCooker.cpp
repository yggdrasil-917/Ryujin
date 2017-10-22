
#include "RyujinCooker.hpp"



namespace Ryujin
{
    RyujinCooker::RyujinCooker(const String& inCookedDir)
    :
    cookedDir(inCookedDir),
    profiler(nullptr)
    {
        
    }
    
    RyujinCooker::~RyujinCooker()
    {
        SAFE_DELETE(profiler);
    }
    
    
    void RyujinCooker::Initialize(ProfilerType profType)
    {
        switch (profType)
        {
            case ProfilerType::PT_Chrome:
                profiler = RYUJIN_NEW ChromeProfiler();
                profiler->Init();
                break;
                
            case ProfilerType::PT_Pix:
                profiler = RYUJIN_NEW PixProfiler();
                profiler->Init();
                break;
        }
    }
    
    void RyujinCooker::Shutdown()
    {
        if (Profiler::IsProfiling())
        {
            profiler->Shutdown();
        }
    }
    
    void RyujinCooker::CookEngineDefaults()
    {
        SCOPED_PROFILE_EVENT(CookingEngineDefaults, PC_Cooker, 255, 255, 0);
        
        // Default texture
        CookableItem item;
        item.sourceFilename = String::Printf("%sdefault.png", TEXTURE_CONTENT_DIR);
        item.targetFilename = String::Printf("%sdefault.asset", *cookedDir);
        textureCooker.CookTexture(item);
        
        // Default mesh
        const char* meshFile = "cube.obj";
        item.sourceFilename = String::Printf("%s%s", MODEL_CONTENT_DIR, meshFile);
        item.targetFilename = String::Printf("%scube.asset", *cookedDir);
        meshCooker.CookMesh(item, true);
    }
    
    bool RyujinCooker::CookMaps(const DynamicArray<String>& maps)
    {
        SCOPED_PROFILE_EVENT(CookingMaps, PC_Cooker, 255, 255, 0);
        
        
        for (const auto& map : maps)
        {
            UniquePtr<JsonValue> root(Json::ParseFile(*String::Printf("%s%s/world.json", MAP_CONTENT_DIR, *map)));
            RYUJIN_ASSERT(root, "Failed to parse map json");
            
            const JsonObject& sceneRoot = root->AsObject();
            
            worldCooker.CookWorld(sceneRoot, meshCooker, map);
            
        }
        
        meshCooker.CookQueuedMeshes();
        
        return true;
    }
}
