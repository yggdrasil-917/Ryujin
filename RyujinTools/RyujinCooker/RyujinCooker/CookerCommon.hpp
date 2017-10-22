
#ifndef COOKER_COMMON_HPP
#define COOKER_COMMON_HPP



#include "RyujinCore/Logger/Logger.hpp"
#include "RyujinCore/Containers/String.hpp"

#include "RyujinCore/Profiler/Profiler.hpp"
#include "RyujinCore/Profiler/PixProfiler.hpp"
#include "RyujinCore/Profiler/ChromeProfiler.hpp"

namespace Ryujin
{
    struct CookableItem
    {
        String sourceFilename;
        String targetFilename;
        
        friend bool operator ==(const CookableItem& a, const CookableItem& b)
        {
            return a.sourceFilename == b.sourceFilename;
        }
    };
    
    // Relative content directory paths assuming binary is inside Build/$CONFIGURATION/ directory
#define MODEL_CONTENT_DIR       "../../../Content/Models/"
#define TEXTURE_CONTENT_DIR     "../../../Content/Textures/"
#define MATERIAL_CONTENT_DIR    "../../../Content/Materials/"
#define MAP_CONTENT_DIR         "../../../Content/Maps/"
#define CONFIG_CONTENT_DIR      "../../../Content/Config/"
    
#if PLATFORM_MAC
#   define COOKED_CONTENT_DIR      "../../../Content/CookedMac/"
#elif PLATFORM_LINUX
#   define COOKED_CONTENT_DIR      "../../../Content/CookedLinux/"
#else
#   define COOKED_CONTENT_DIR      "../../../Content/CookedPC/"
#endif
}



#endif
