#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP


#include "MemoryTrackingPolicies.hpp"



namespace Ryujin
{
	template<class AllocPolicy, class MemTrackingPolicy, class MemTaggingPolicy>
	class Allocator
	{
	private:
		AllocPolicy allocator;
		MemTrackingPolicy memTracking;

	public:
		Allocator(uint64 sizeInBytes, const char* allocatorName)
        : allocator(AllocPolicy(sizeInBytes, allocatorName)),
          memTracking(MemTrackingPolicy(allocatorName))
		{
		}

		~Allocator()
		{
		}

		FORCEINLINE void* Allocate(PtrSize bytesToAllocate, uint32 alignment = PLATFORM_ALIGNMENT)
		{
			memTracking.IncrementAllocationCount();
			const uint64 currentBytesAllocated = allocator.GetBytesAllocated();
			void* memory = allocator.Allocate(bytesToAllocate, alignment);
			const uint64 totalBytesAllocated = allocator.GetBytesAllocated() - currentBytesAllocated;
			memTracking.IncrementBytesAllocated(totalBytesAllocated);
			MemTaggingPolicy::TagAllocated(memory, totalBytesAllocated);

			return memory;
		}

		FORCEINLINE void Free(void* data)
		{
			memTracking.DecrementAllocationCount();
			const uint64 currentBytesAllocated = allocator.GetBytesAllocated();
			allocator.Free(data);
			const uint64 totalBytesDeallocated = currentBytesAllocated - allocator.GetBytesAllocated();
			memTracking.DecrementBytesAllocated(totalBytesDeallocated);
			MemTaggingPolicy::TagFreed(data, totalBytesDeallocated);
		}

		FORCEINLINE void Reset()
		{
            // Only LinearAllocator uses this
			memTracking.Reset();
			allocator.Reset();
			MemTaggingPolicy::TagFreed(allocator.GetStart(), allocator.GetFreeBytes());
		}

		FORCEINLINE uint8* GetStart() const
		{
			return allocator.GetStart();
		}

		FORCEINLINE uint8* GetEnd() const
		{
			return allocator.GetEnd();
		}

		FORCEINLINE PtrSize GetBytesAllocated() const
		{
			return allocator.GetBytesAllocated();
		}

		FORCEINLINE PtrSize GetFreeBytes() const
		{
			return allocator.GetFreeBytes();
		}

		FORCEINLINE const char* GetAllocatorName() const
		{
			return allocator.GetAllocatorName();
		}
        
        FORCEINLINE AllocPolicy* GetAllocationPolicy() const
        {
            return allocator;
        }
	};
    
#if USE_DEBUG_ALLOCATORS
    template<class AllocatorPolicy>
    using RyujinAllocator = Allocator<AllocatorPolicy, AllocationTracking, NoMemTagging>;
#elif STATS_ENABLED
    template<class AllocatorPolicy>
    using RyujinAllocator = Allocator<AllocatorPolicy, AllocationTracking, NoMemTagging>;
#else
    template<class AllocatorPolicy>
    using RyujinAllocator = Allocator<AllocatorPolicy, NoAllocationTracking, NoMemTagging>;
#endif
}


#endif