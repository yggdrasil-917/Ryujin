
#include "GlobalAllocators.hpp"
#include "../Logger/Logger.hpp"

using namespace Ryujin;

static void StaticDumpUnfreedMemory(void)
{
    AllocList::Instance()->DumpUnfreedMemory();
}

AllocList::AllocList()
: bHasRegisteredWithAtExit(false)
{
	allocList = new List<AllocInfo>();
}

void AllocList::AddAlloc(uintptr inAddress, PtrSize inAllocSize, const char *inFile, PtrSize inLine)
{
    if (!bHasRegisteredWithAtExit)
    {
        std::atexit(StaticDumpUnfreedMemory);
        bHasRegisteredWithAtExit = true;
    }
    AllocInfo info;
    info.address = inAddress;
    info.allocSize = inAllocSize;
    strncpy(info.file, inFile, 127);
    info.line = inLine;
    allocList->Add(info);
}


void AllocList::RemoveAlloc(uintptr inAddress)
{
    if (!bHasRegisteredWithAtExit)
    {
        std::atexit(StaticDumpUnfreedMemory);
        bHasRegisteredWithAtExit = true;
    }
    AllocInfo info;
    if (allocList->GetFirst(info))
    {
        do
        {
            if (info.address == inAddress)
            {
                allocList->Remove(info);
                return;
            }
        } while (allocList->GetNext(info));
    }
}


void AllocList::DumpUnfreedMemory()
{
    Warnf(Channel::Memory, "------------ Memory leak report");
    bool bLeakedMemory = false;
    AllocInfo info;
    PtrSize totalBytesUnfreed = 0;
    if (allocList->GetFirst(info))
    {
        do
        {
            bLeakedMemory = true;
            totalBytesUnfreed += info.allocSize;
            Warnf(Channel::Memory, "Leaked Memory: %s(%d) address %x bytes %d", info.file, info.line, info.address, info.allocSize);
        } while(allocList->GetNext(info));
    }
    
    if (!bLeakedMemory)
    {
        Warnf(Channel::Memory, "No Memory leaked");
    }
    else
    {
        Warnf(Channel::Memory, "Total bytes leaked: %llu", totalBytesUnfreed);
    }
    
    Warnf(Channel::Memory, "------------ End memory leak report");
}

#if USE_DEBUG_ALLOCATORS

void* GlobalNew(Ryujin::PtrSize count, Ryujin::int32 line, const char* file)
{
#if defined(__OBJC__)
    void* p = malloc(count);
#else
    void* p = Ryujin::DefaultEngineAllocator::Instance().Allocate(count); 
#endif
    
    RYUJIN_ASSERT(p, "Failed to allocate memory");
    FileLog(Channel::Memory, "ALLOC: address %p -> %d bytes, line %d of file %s", p, count, line, file);
    AllocList::Instance()->AddAlloc(reinterpret_cast<uintptr>(p), count, file, line);
    
    return p;
}

void* GlobalNewArray(Ryujin::PtrSize count, Ryujin::int32 line, const char* file)
{
#if defined(__OBJC__)
    void* p = malloc(count);
#else
    void* p = Ryujin::DefaultEngineAllocator::Instance().Allocate(count);
#endif
    
    RYUJIN_ASSERT(p, "Failed to allocate memory");
    FileLog(Channel::Memory, "ALLOC: address %p -> %d bytes, line %d of file %s", p, count, line, file);
    AllocList::Instance()->AddAlloc(reinterpret_cast<uintptr>(p), count, file, line);
    
    return p;
}

void GlobalDelete(void* p) noexcept
{
    RYUJIN_ASSERT(p, "Passed null pointer to free");
    FileLog(Channel::Memory, "Freeing address %p", p);
    AllocList::Instance()->RemoveAlloc(reinterpret_cast<uintptr>(p));
    
#if defined(__OBJC__)
    free(p);
#else
    Ryujin::DefaultEngineAllocator::Instance().Free(p);
#endif
}

void GlobalDeleteArray(void* p) noexcept
{
    RYUJIN_ASSERT(p, "Passed null pointer to free");
    FileLog(Channel::Memory, "Freeing address %p", p);
    AllocList::Instance()->RemoveAlloc(reinterpret_cast<uintptr>(p));
    
#if defined(__OBJC__)
    free(p);
#else
    Ryujin::DefaultEngineAllocator::Instance().Free(p);
#endif
}


#else

void* GlobalNew(Ryujin::PtrSize count)
{
#if defined(__OBJC__)
    void* p = malloc(count);
#else
    void* p = Ryujin::DefaultEngineAllocator::Instance().Allocate(count);
#endif
    
    RYUJIN_ASSERT(p, "Failed to allocate memory");
    
    return p;
}

void* GlobalNewArray(Ryujin::PtrSize count)
{
#if defined(__OBJC__)
    void* p = malloc(count);
#else
    void* p = Ryujin::DefaultEngineAllocator::Instance().Allocate(count);
#endif
    
    RYUJIN_ASSERT(p, "Failed to allocate memory");
    
    return p;
}

void GlobalDelete(void* p) noexcept
{
    RYUJIN_ASSERT(p, "Passed null pointer to free");
    
#if defined(__OBJC__)
    free(p);
#else
    Ryujin::DefaultEngineAllocator::Instance().Free(p);
#endif
}

void GlobalDeleteArray(void* p) noexcept
{
    RYUJIN_ASSERT(p, "Passed null pointer to free");
    
#if defined(__OBJC__)
    free(p);
#else
    Ryujin::DefaultEngineAllocator::Instance().Free(p);
#endif
}
#endif