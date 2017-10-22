
#pragma once


#include "Memory.hpp"
#include "Allocator.hpp"
#include "../Threading/Thread.hpp"


namespace Ryujin
{
    class AnsiAllocator
    {
    private:
        uint64 bytesAllocated;
        const char* allocatorName;
        
    public:
        AnsiAllocator(uint64 sizeInBytes = 0, const char* allocatorName = "AnsiAllocator");
        ~AnsiAllocator();
        
        AnsiAllocator(AnsiAllocator&& other)
        {
            bytesAllocated = other.bytesAllocated;
            allocatorName = Move(other.allocatorName);
            
            other.allocatorName = nullptr;
        }
        
        AnsiAllocator& operator=(AnsiAllocator&& other)
        {
            bytesAllocated = other.bytesAllocated;
            allocatorName = Move(other.allocatorName);
            
            other.allocatorName = nullptr;
            
            return *this;
        }
        
        
        void* Allocate(PtrSize bytesToAllocate, uint32 alignment = PLATFORM_ALIGNMENT);
        
        void Free(void* data);
        
        void Reset();
        
        FORCEINLINE uint8* GetStart() const { return nullptr; }
        FORCEINLINE uint8* GetEnd() const { return nullptr; }
        FORCEINLINE uint64 GetBytesAllocated() const { return bytesAllocated; }
        FORCEINLINE PtrSize GetFreeBytes() const { return 0; }
        FORCEINLINE const char* GetAllocatorName() const { return allocatorName; }
    };
    
    FORCEINLINE AnsiAllocator::AnsiAllocator(uint64 sizeInBytes, const char* name)
    {
        allocatorName = name;
        bytesAllocated = 0;
    }
    
    FORCEINLINE AnsiAllocator::~AnsiAllocator()
    {
        
    }
    
    FORCEINLINE void* AnsiAllocator::Allocate(PtrSize bytesToAllocate, uint32 alignment)
    {
        // On mac sometimes the event system would try to allocate 0 bytes which the system malloc can handle
        bytesToAllocate = MathUtils::Max<PtrSize>(bytesToAllocate, PLATFORM_ALIGNMENT);
#if STATS_ENABLED
        bytesToAllocate = Memory::GetAlignedSize(bytesToAllocate + sizeof(PtrSize), alignment);
        bytesAllocated += bytesToAllocate;
        uint8* p = (uint8*)Memory::AlignedAllocate(bytesToAllocate, alignment);
		Memory::MemCpy(p, &bytesToAllocate, sizeof(PtrSize));
        //*p = bytesToAllocate;
        void* ptr = (p + sizeof(PtrSize));
        return ptr;
#else
        return Memory::AlignedAllocate(bytesToAllocate, alignment);
#endif
    }
    
    FORCEINLINE void AnsiAllocator::Free(void* data)
    {
#if STATS_ENABLED
        uint8* p = (uint8*)data;
        p = (p - sizeof(PtrSize));
        PtrSize allocSize = (PtrSize)*p;
        bytesAllocated -= allocSize;
        Memory::AlignedFree(p);
#else
        Memory::AlignedFree(data);
#endif
    }
    
    FORCEINLINE void AnsiAllocator::Reset()
    {
        
    }
    
    class CORE_API AnsiGlobalAllocator
    {
    public:
        static RyujinAllocator<AnsiAllocator>& Instance()
        {
            static RyujinAllocator<AnsiAllocator> allocator(0, "Global default allocator");
            return allocator;
        }
    };
}


