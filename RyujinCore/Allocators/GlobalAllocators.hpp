
#pragma once


#include "Memory.hpp"

#if USE_TBB
#include "TBBAllocator.hpp"
#else
#include "AnsiAllocator.hpp"
#endif

#include "../Containers/List.hpp"
#include "../CoreUtils/Singleton.hpp"

namespace Ryujin
{
#if USE_TBB
    class TBBGlobalAllocator;
    using DefaultEngineAllocator = TBBGlobalAllocator;
#else
    class AnsiGlobalAllocator;
    using DefaultEngineAllocator = AnsiGlobalAllocator;
#endif
    
    class CORE_API AllocList : public Singleton<AllocList>
    {
    public:
        struct AllocInfo
        {
            uintptr address;
            PtrSize allocSize;
            char file[128];
            PtrSize line;
            
            bool operator==(const AllocInfo& other) const { return address == other.address; }
        };
        
    private:
        List<AllocInfo>* allocList;
        bool bHasRegisteredWithAtExit;
        
    public:
        AllocList();
        
        void AddAlloc(uintptr inAddress, PtrSize inAllocSize, const char* inFile, PtrSize inLine);
        void RemoveAlloc(uintptr inAddress);
        void DumpUnfreedMemory();
    };
}

