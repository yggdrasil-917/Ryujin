#include "ModuleMac.hpp"

#include "RyujinCore/Containers/String.hpp"

#include <dlfcn.h>

#define DLL_EXTENSION ".dylib"
#define PLUGIN_PATH "/"


namespace Ryujin
{
    ModuleMac::~ModuleMac()
    {
        
    }
    
    void ModuleMac::Initialize()
    {
        String path = String::Printf("%s%s.dylib", PLUGIN_PATH, name);
        handle = ::dlopen(*path, RTLD_LAZY);
    }
    
    void ModuleMac::Shutdown()
    {
        if (handle)
        {
            ::dlclose(handle);
            handle = nullptr;
        }
    }
}
