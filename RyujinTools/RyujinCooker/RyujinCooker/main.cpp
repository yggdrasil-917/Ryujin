

#include "RyujinCore/Logger/Logger.hpp"
#include "RyujinCore/CoreUtils/Json.hpp"
#include "RyujinCore/OS/FileSystem.hpp"
#include "RyujinCore/OS/Process.hpp"

#include "RyujinCooker.hpp"


using namespace Ryujin;



int main(int argc, char** argv)
{
    DynamicArray<String> mapList;
    ProfilerType profType = ProfilerType::PT_None;
    bool bFullRecook = false;
    for (int32 i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-mapList") == 0)
        {
            ++i;
            while (i < argc && argv[i][0] != '-')
            {
                mapList.Add(argv[i]);
                ++i;
            }
            --i;
        }
        else if (strcmp(argv[i], "-chromeProfile") == 0)
        {
            profType = ProfilerType::PT_Chrome;
        }
        else if (strcmp(argv[i], "-pixProfile") == 0)
        {
            profType = ProfilerType::PT_Pix;
        }
        else if (strcmp(argv[i], "-recook") == 0)
        {
            bFullRecook = true;
        }
    }
    
    if (bFullRecook)
    {
        //Delete contents of cooked dir 
        Directory::DeleteDirectory(COOKED_CONTENT_DIR);
    }
    
    if (!Directory::DirectoryExists(COOKED_CONTENT_DIR))
    {
        Directory::CreateDirectory(COOKED_CONTENT_DIR);
    }
    
    RyujinCooker cooker(COOKED_CONTENT_DIR);
    cooker.Initialize(profType);
    
    DynamicArray<String> maps;
    for (const auto& map : mapList)
    {
        maps.Add(String::Printf("%s%s/world.json", MAP_CONTENT_DIR, *map));
    }
    
    Debugf(Info, "******** Cooking engine defaults");
    cooker.CookEngineDefaults();
    
	Debugf(Info, "******** Cooking %d maps\n\n", maps.GetSize());
    cooker.CookMaps(mapList);
    
	Debugf(Info, "Shutting down cooker");
    cooker.Shutdown();
    
    
    return 0;
}
