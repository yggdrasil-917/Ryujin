
#ifndef RYUJIN_COOKER_HPP
#define RYUJIN_COOKER_HPP

#include "CookerCommon.hpp"
#include "MeshCooker.hpp"
#include "TextureCooker.hpp"
#include "WorldCooker.hpp"

#include "RyujinCore/CoreUtils/Json.hpp"
#include "RyujinCore/CoreUtils/SmartPointer.hpp"



namespace Ryujin
{
    class RyujinCooker
    {
    private:
        MeshCooker meshCooker;
        TextureCooker textureCooker;
        WorldCooker worldCooker;
        String cookedDir;
        Profiler* profiler;
        
    public:
        RyujinCooker(const String& inCookedDir);
        ~RyujinCooker();
        
        void Initialize(ProfilerType profType);
        void Shutdown();
        
        void CookEngineDefaults();
        bool CookMaps(const DynamicArray<String>& maps);
    };
}


#endif
