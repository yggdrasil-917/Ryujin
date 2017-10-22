#ifndef MEMORY_POOL_HPP
#define MEMORY_POOL_HPP

#include "Memory.hpp"
#include "../Containers/SlotArray.hpp"

namespace Ryujin
{
	template<class T>
    class MemoryPool : public NonCopyable
	{
	private:
		float poolSizeInKb;
		uint32 numObjects;
		uint32 numAllocatedElements;
        T* pool;
        SlotArray<int32> freeList;
        int32 currentFreeIndex;

	public:
        MemoryPool() : poolSizeInKb(0), numObjects(0), numAllocatedElements(0), pool(nullptr), currentFreeIndex(0) {}
        ~MemoryPool() { SAFE_DELETE_ARRAY(pool); }

		void SetPoolSize(uint32 numPoolObjects);
		T* Allocate();
		void Free(T*& object);

		FORCEINLINE float GetPoolSizeInKb() const { return poolSizeInKb; }
		FORCEINLINE uint32 GetPoolObjectCapacity() const { return numObjects; }
		FORCEINLINE uint32 GetNumAllocatedObjects() const { return numAllocatedElements; }
	};

	template<class T>
	void MemoryPool<T>::SetPoolSize(uint32 numPoolObjects)
	{
		poolSizeInKb = numPoolObjects * sizeof(T) / 1024.0f;
        SAFE_DELETE_ARRAY(pool);
        freeList.Resize(numPoolObjects);

		numObjects = numPoolObjects;
		pool = RYUJIN_NEW T[numPoolObjects];
        for (int32 i = 0; i < numObjects; ++i)
        {
            freeList.Allocate(i);
        }

        currentFreeIndex = 0;
	}

	template<class T>
	T* MemoryPool<T>::Allocate()
	{
		if (currentFreeIndex == -1)
			return nullptr;

        uint32 index = freeList[0];
        freeList.Free(0);
		T* object = &pool[index];
		++numAllocatedElements;

		return object;
	}

	template<class T>
	void MemoryPool<T>::Free(T*& object)
	{
        int32 index = ((uintptr)object - (uintptr)pool) / sizeof(T);
        RYUJIN_ASSERT(index >= 0 && index < numObjects, "Index out of bounds");
        
        object = nullptr;
        freeList.Allocate(index);
		--numAllocatedElements;
	}
}



#endif
