
#ifndef SPARSE_ARRAY_HPP
#define SPARSE_ARRAY_HPP


#include "DynamicArray.hpp"


namespace Ryujin
{
    
    template<class T, typename GenericHandle, class Allocator = DefaultEngineAllocator>
    class SparseArray
    {
    private:
        
        struct Object
        {
            T item;
            GenericHandle handle;
        };
        
        Object* objects;
        int32 numObjects;
        int32 objectCapacity;
        DynamicArray<int32, Allocator> sparseToDense;
        DynamicArray<int32, Allocator> denseToSparse;
        int32 firstFreeIndex;
        
    public:
        using Value = Object;
        using Iterator = Value*;
        using ConstIterator = const Value*;
        using Reference = Value&;
        using ConstReference = const Value&;
        
        // STL compatibility
        using iterator = Iterator;
        using const_iterator = ConstIterator;
        using reference = Reference;
        using const_reference = ConstReference;
        
        SparseArray()
        : numObjects(0)
        {
//            objects = new Object[8];
            objects = (Object*)Allocator::Instance().Allocate(8 * sizeof(Object));
            Memory::MemSet(objects, 0, 8 * sizeof(Object));
            objectCapacity = 8;
            firstFreeIndex = -1;
        }
        
        ~SparseArray()
        {
//            SAFE_DELETE_ARRAY(objects);
            Allocator::Instance().Free(objects);
        }
        
        FORCEINLINE bool Contains(GenericHandle handle) const
        {
            const int32& sparseIndex = sparseToDense[handle.index];
            return objects[sparseIndex].handle.generation == handle.generation;
        }
        
        FORCEINLINE T& GetObject(GenericHandle handle) const
        {
            return objects[ sparseToDense[handle.index] ].item;
        }
        
        FORCEINLINE void SetObject(GenericHandle handle, T& value)
        {
            objects[ sparseToDense[handle.index] ].item = value;
        }
        
        FORCEINLINE void MoveObject(GenericHandle handle, T&& value)
        {
            objects[ sparseToDense[handle.index] ].item = Move(value);
        }
        
        FORCEINLINE GenericHandle Add()
        {
            int32 denseIndex = numObjects++;
            int32 sparseIndex = firstFreeIndex;
            
            if (sparseIndex >= 0)
            {
                firstFreeIndex = sparseToDense[sparseIndex];
                sparseToDense[sparseIndex] = denseIndex;
            }
            else
            {
                sparseIndex = sparseToDense.GetSize();
                sparseToDense.Add(denseIndex);
            }
            
            denseToSparse.Add(sparseIndex);
            
            if (numObjects == objectCapacity)
            {
                int32 newSize = objectCapacity * 2 + 1;
                Object* newData = (Object*)Allocator::Instance().Allocate(newSize * sizeof(Object));
                Memory::MemCpy(newData, objects, numObjects * sizeof(Object));
                
                int32 slack = MathUtils::Max<int32>(newSize - numObjects, 0);
                if (slack > 0)
                    Memory::MemSet(newData + numObjects, 0, slack * sizeof(Object));
                
                Allocator::Instance().Free(objects);
                objects = newData;
                objectCapacity = newSize;
            }
            
            objects[denseIndex].handle.index = sparseIndex;
            objects[denseIndex].handle.generation++;
            
            return objects[denseIndex].handle;
        }
        
        FORCEINLINE void Remove(GenericHandle handle)
        {
            int32 denseIndex = sparseToDense[handle.index];
            int32 lastElementSparseIndex = denseToSparse.GetSize() - 1;
            denseToSparse.Remove(denseToSparse.GetSize() - 1);
            
            MoveSwap<Object>(Move(objects[denseIndex]), Move(objects[numObjects - 1]));
            --numObjects;
            
            sparseToDense[lastElementSparseIndex] = denseIndex;
            denseToSparse[denseIndex] = lastElementSparseIndex;
            sparseToDense[handle.index] = firstFreeIndex;
            firstFreeIndex = handle.index;
        }
        
        FORCEINLINE uint32 GetCount() const { return numObjects; }
        
        /// Make it compatible with the C++11 range based for loop.
        FORCEINLINE Iterator Begin()
        {
            return objects;
        }
        
        /// Make it compatible with the C++11 range based for loop.
        FORCEINLINE ConstIterator Begin() const
        {
            return objects;
        }
        
        /// Make it compatible with the C++11 range based for loop.
        FORCEINLINE Iterator End()
        {
            return objects + numObjects;
        }
        
        /// Make it compatible with the C++11 range based for loop.
        FORCEINLINE ConstIterator End() const
        {
            return objects + numObjects;
        }
        
        /// Make it compatible with the C++11 range based for loop.
        FORCEINLINE Iterator begin()
        {
            return Begin();
        }
        
        /// Make it compatible with the C++11 range based for loop.
        FORCEINLINE ConstIterator begin() const
        {
            return Begin();
        }
        
        /// Make it compatible with the C++11 range based for loop.
        FORCEINLINE Iterator end()
        {
            return End();
        }
        
        /// Make it compatible with the C++11 range based for loop.
        FORCEINLINE ConstIterator end() const
        {
            return End();
        }
        
        /// Get first element.
        FORCEINLINE Reference Front()
        {
            return objects[0];
        }
        
        /// Get first element.
        FORCEINLINE ConstReference Front() const
        {
            return objects[0];
        }
        
        /// Get last element.
        FORCEINLINE Reference Back()
        {
            return objects[numObjects - 1];
        }
        
        /// Get last element.
        FORCEINLINE ConstReference Back() const
        {
            return objects[numObjects - 1];
        }
        
        /// Get last element.
        FORCEINLINE ConstReference back() const
        {
            return objects[numObjects - 1];
        }
    };
    
}



#endif
