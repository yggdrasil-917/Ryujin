#pragma once


#include "IModule.hpp"


namespace Ryujin
{
    class EXTENSIONS_API ModuleMac : public IModule
    {
    public:
        ~ModuleMac();
        
        void Initialize() OVERRIDE FINAL;
        void Shutdown() OVERRIDE FINAL;
    };
}
