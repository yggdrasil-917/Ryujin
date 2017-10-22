#ifndef TBB_ALLOCATOR_HPP
#define TBB_ALLOCATOR_HPP


//#include "Memory.hpp"
#include "Allocator.hpp"
#include "../Threading/Thread.hpp"

#include "../Math/MathUtils.hpp"

#include "tbb/scalable_allocator.h"

namespace Ryujin
{
    
    class TBBAllocator
	{
    private:
        AtomicVar<uint64> bytesAllocated;
        const char* allocatorName;

    public:
        TBBAllocator(uint64 sizeInBytes = 0, const char* allocatorName = "TBBAllocator");
        ~TBBAllocator();
        
        TBBAllocator(TBBAllocator&& other)
        {
            bytesAllocated = other.bytesAllocated.GetValue();
            allocatorName = Move(other.allocatorName);
            
            other.allocatorName = nullptr;
        }
        
        TBBAllocator& operator=(TBBAllocator&& other)
        {
            bytesAllocated = other.bytesAllocated.GetValue();
            allocatorName = Move(other.allocatorName);
            
            other.allocatorName = nullptr;
            
            return *this;
        }
        
        
        void* Allocate(PtrSize bytesToAllocate, uint32 alignment = PLATFORM_ALIGNMENT);
        
        void Free(void* data);
        
        void Reset();
        
        FORCEINLINE uint8* GetStart() const { return nullptr; }
        FORCEINLINE uint8* GetEnd() const { return nullptr; }
        FORCEINLINE PtrSize GetBytesAllocated() const { return bytesAllocated.GetValue(); }
        FORCEINLINE PtrSize GetFreeBytes() const { return 0; }
        FORCEINLINE const char* GetAllocatorName() const { return allocatorName; }
	};
    
    FORCEINLINE TBBAllocator::TBBAllocator(uint64 sizeInBytes, const char* name)
    {
        allocatorName = name;
        bytesAllocated = 0;
    }
    
    FORCEINLINE TBBAllocator::~TBBAllocator()
    {
        
    }
    
    FORCEINLINE void* TBBAllocator::Allocate(PtrSize bytesToAllocate, uint32 alignment)
    {
        // On mac sometimes the event system would try to allocate 0 bytes which the system malloc can handle
        bytesToAllocate = MathUtils::Max<PtrSize>(bytesToAllocate, PLATFORM_ALIGNMENT);
        
//        bytesToAllocate = Memory::GetAlignedSize(bytesToAllocate + sizeof(PtrSize), alignment);
#if STATS_ENABLED
        bytesAllocated.Increment(bytesToAllocate);
#endif
//        uint8* p = (uint8*)scalable_aligned_malloc(bytesToAllocate, alignment);
//        *p = bytesToAllocate;
//        void* ptr = (p + sizeof(PtrSize));
//        return ptr;

        return scalable_aligned_malloc(bytesToAllocate, alignment);
    }
    
    FORCEINLINE void TBBAllocator::Free(void* data)
    {
#if STATS_ENABLED
        PtrSize dataSize = scalable_msize(data);
        bytesAllocated.Decrement(dataSize);
        
//        uint8* p = (uint8*)data;
//        p = (p - sizeof(PtrSize));
//        PtrSize allocSize = (PtrSize)*p;
//        bytesAllocated.Decrement(allocSize);
//        scalable_aligned_free(p);
#endif
        scalable_aligned_free(data);
    }
    
    FORCEINLINE void TBBAllocator::Reset()
    {
        
    }
    
    
    class CORE_API TBBGlobalAllocator
    {
    public:
        static RyujinAllocator<TBBAllocator>& Instance()
        {
            static RyujinAllocator<TBBAllocator> allocator(0, "Global TBB allocator");
            return allocator;
        }
    };
    
}

#endif
