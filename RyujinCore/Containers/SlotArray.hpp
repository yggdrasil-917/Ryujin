
#ifndef SLOT_ARRAY_HPP
#define SLOT_ARRAY_HPP


//#include "../Allocators/Memory.hpp"
//#include "../Allocators/AnsiAllocator.hpp"
//#include "../Allocators/TBBAllocator.hpp"
//#include "../Allocators/Allocator.hpp"

#include "../Allocators/GlobalAllocators.hpp"

namespace Ryujin
{
    template<typename T, class Allocator = DefaultEngineAllocator>
    class SlotArray
    {
    private:
        T* data;
        uint32 size;
        uint32 capacity;
        
        void Move_Interal(uint32 to, uint32 from)
        {
            data[to] = Move(data[from]);
        }
        
    public:
        SlotArray()
        :
        data(nullptr),
        size(0),
        capacity(0)
        {
            
        }
        
        SlotArray(uint32 inSize)
        {
            size = 0;
            capacity = inSize;
            data = (T*)DefaultEngineAllocator::Instance().Allocate(capacity * sizeof(T));
        }
        
        template<typename...Args>
        uint32 Allocate(Args...args)
        {
            if (size == capacity)
                Resize(capacity * 2);
            
            new (data + size) T(args...);
            
            return size++;
        }
        
        void Free(uint32 index)
        {
            RYUJIN_ASSERT(index < capacity, "Index out of bounds");
            
            T* value = (data + index);
            value->~T();
            
            Move_Interal(index, size - 1);
            --size;
        }
        
//        void Free(T* value)
//        {
//            uint32 index = (value - data) / sizeof(T);
//            RYUJIN_ASSERT(index >= 0 && index < capacity, "Index out of bounds");
//            
//            T* entry = (data + index);
//            entry->~T();
//            
//            Move_Interal(index, size - 1);
//            --size;
//        }
        
        void Resize(uint32 newSize)
        {
            if (newSize == capacity)
                return;
            
            capacity = newSize;
            T* newData = (T*)DefaultEngineAllocator::Instance().Allocate(newSize * sizeof(T));
            
            if (size > 0)
                Memory::MemCpy(newData, data, sizeof(T) * size);
            
            DefaultEngineAllocator::Instance().Free(data);
            data = newData;
        }
        
        T& operator[](uint32 index)
        {
            RYUJIN_ASSERT(index < capacity, "Index out of bounds");
            return data[index];
        }
        
        const T& operator[](uint32 index) const
        {
            RYUJIN_ASSERT(index < capacity, "Index out of bounds");
            return data[index];
        }
        
        FORCEINLINE uint32 GetSize() const { return size; }
    };
}



#endif
