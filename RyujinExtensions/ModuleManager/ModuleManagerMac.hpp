#pragma once


#include "ModuleManager.hpp"



namespace Ryujin
{
    class EXTENSIONS_API ModuleManagerMac : public ModuleManager
    {
    public:
        ~ModuleManagerMac();
        
        bool LoadModule(const char* moduleName) OVERRIDE FINAL;
        void RemoveModule(uint64 moduleId) OVERRIDE FINAL;
        ModuleRef FindModule(uint64 moduleId) OVERRIDE FINAL;
        
        static ModuleManagerMac* GetInstance();
    };
}
