#ifndef MEMORY_TRACKING_POLICIES_HPP
#define MEMORY_TRACKING_POLICIES_HPP


#include "Memory.hpp"


namespace Ryujin
{
	class AllocationTracking
	{
	private:
		uint32 numAllocations;
		PtrSize bytesAllocated;
		const char* allocatorName;

	public:
		AllocationTracking()
		{

		}

		AllocationTracking(const char* inAllocatorName)
			: 
			numAllocations(0),
			bytesAllocated(0),
			allocatorName(inAllocatorName)
		{

		}

		FORCEINLINE void IncrementAllocationCount() 
		{
			++numAllocations;
		}

		FORCEINLINE void DecrementAllocationCount()
		{
			--numAllocations;
		}

		FORCEINLINE void IncrementBytesAllocated(PtrSize inBytesAllocated)
		{
			bytesAllocated += inBytesAllocated;
		}

		FORCEINLINE void DecrementBytesAllocated(PtrSize inBytesAllocated)
		{
			bytesAllocated -= inBytesAllocated;
		}

		FORCEINLINE void Reset()
		{
			numAllocations = 0;
			bytesAllocated = 0;
		}
        
        FORCEINLINE uint32 GetAllocationCount() const
        {
            return numAllocations;
        }
        
        FORCEINLINE PtrSize GetBytesAllocated() const
        {
            return bytesAllocated;
        }

		FORCEINLINE const char* GetAllocatorTrackingName() const
		{
			return allocatorName;
		}
	};


	class NoAllocationTracking
	{
	public:
		NoAllocationTracking()
		{

		}

		NoAllocationTracking(const char* inAllocatorName)
		{

		}

		FORCEINLINE void IncrementAllocationCount()
		{
		}

		FORCEINLINE void DecrementAllocationCount()
		{
		}

		FORCEINLINE void IncrementBytesAllocated(PtrSize inBytesAllocated)
		{
		}

		FORCEINLINE void DecrementBytesAllocated(PtrSize inBytesAllocated)
		{
		}

		FORCEINLINE void Reset()
		{
        }
        
        FORCEINLINE uint32 GetAllocationCount() const
        {
            return 0;
        }
        
        FORCEINLINE PtrSize GetBytesAllocated() const
        {
            return 0;
        }

		FORCEINLINE const char* GetAllocatorTrackingName() const
		{
			return "NoAllocationTracking";
		}
	};


	class MemTagging
	{
	private:
		static constexpr const uint8 allocated = 0xFD;
		static constexpr const uint8 freed = 0xFE;

	public:
		static void TagAllocated(void* allocation, PtrSize allocationSize)
		{
			Memory::MemSet(allocation, allocated, allocationSize);
		}

		static void TagFreed(void* allocation, PtrSize allocationSize)
		{
			Memory::MemSet(allocation, freed, allocationSize);
		}
	};

	class NoMemTagging
	{
	public:
		static void TagAllocated(void* allocation, PtrSize allocationSize)
		{
		}

		static void TagFreed(void* allocation, PtrSize allocationSize)
		{
		}
	};
}


#endif