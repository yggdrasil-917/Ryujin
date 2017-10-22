#pragma once

#include "../Utils/ExtensionsBase.hpp"

#include "RyujinCore/CoreUtils/SmartPointer.hpp"


namespace Ryujin
{
#if PLATFORM_WINDOWS
    using ModuleHandle = HMODULE;
#else
    using ModuleHandle = void*;
#endif
    
    class EXTENSIONS_API IModule
    {
    protected:
        const char* name;
        ModuleHandle handle;
        
    public:
        VIRTUAL ~IModule() {}
        
        VIRTUAL void Initialize() = 0;
        VIRTUAL void Shutdown() = 0;
        
        FORCEINLINE const char* GetName() const { return name; }
    };
    
    using ModuleRef = SharedPtr<IModule>;
}
