#include "ModuleManagerMac.hpp"
#include "ModuleMac.hpp"


namespace Ryujin
{
    ModuleManagerMac::~ModuleManagerMac()
    {
        
    }
    
    bool ModuleManagerMac::LoadModule(const char *moduleName)
    {
        return true;
    }
    
    void ModuleManagerMac::RemoveModule(uint64 moduleId)
    {
        
    }
    
    ModuleRef ModuleManagerMac::FindModule(uint64 moduleId)
    {
        ModuleRef r(nullptr);
        modules.TryGetValue(moduleId, r);
        return r;
    }
    
    ModuleManagerMac* ModuleManagerMac::GetInstance()
    {
        static ModuleManagerMac mm;
        return &mm;
    }
}
