#ifndef LINEAR_ALLOCATOR_MEMMARK_HPP
#define LINEAR_ALLOCATOR_MEMMARK_HPP


#include "LinearAllocator.hpp"


namespace Ryujin
{
	class LinearAllocatorMemMark : public NonCopyable
	{
	private:
		LinearAllocator* allocator;
		uint64 totalBytesAllocatedBeforeNewAllocations;

	public:
		LinearAllocatorMemMark(LinearAllocator* inAllocator)
			: allocator(inAllocator)
		{
			totalBytesAllocatedBeforeNewAllocations = allocator->GetBytesAllocated();
		}

		~LinearAllocatorMemMark()
		{
			allocator->SetWritePtrPosition(totalBytesAllocatedBeforeNewAllocations);
		}
	};
}


#endif