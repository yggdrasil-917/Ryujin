#pragma once


#include "../Utils/ExtensionsBase.hpp"

#include "RyujinCore/Containers/HashTable.hpp"
#include "IModule.hpp"


namespace Ryujin
{
    class EXTENSIONS_API ModuleManager
    {
    protected:
        HashTable<uint64, ModuleRef> modules;
        
    public:
        VIRTUAL bool LoadModule(const char* moduleName) = 0;
        VIRTUAL void RemoveModule(uint64 moduleId) = 0;
        VIRTUAL ModuleRef FindModule(uint64 moduleId) = 0;
    };
}
