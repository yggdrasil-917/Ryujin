#pragma once


#include "DynamicArray.hpp"
#include "FixedArray.hpp"

#include <array>


namespace Ryujin
{
    template<typename T>
    class ArrayView
    {
    private:
        T* array;
        PtrSize count;
        
    public:
        ArrayView(T* inArray, PtrSize inSize)
        : array(inArray),
          count(inSize)
        {
            
        }
        
        FORCEINLINE T* GetArray() const { return array; }
        FORCEINLINE PtrSize GetSize() const { return count; }
        FORCEINLINE PtrSize GetByteSize() const { return count * sizeof(T); }
        FORCEINLINE PtrSize GetTypeSize() const { return sizeof(T); }
        
        T& operator[](PtrSize i) const
        {
            RYUJIN_ASSERT(i < count, "Index out of bounds");
            return array[i];
        }
        
        // STL compatibility
        using Value = T;
        using ConstIterator = const Value*;
        using ConstReference = const Value&;
        using const_iterator = ConstIterator;
        using const_reference = ConstReference;
        
        
        ConstIterator Begin() const
        {
            return &array[0];
        }
        
        ConstIterator End() const
        {
            return &array[0] + count;
        }
        
        ConstReference Front() const
        {
            return array[0];
        }
        
        ConstReference Back() const
        {
            return array[count - 1];
        }
        
        // Start of STL compatibility functions
        ConstIterator begin() const
        {
            return Begin();
        }
        
        ConstIterator end() const
        {
            return End();
        }
        
        ConstReference front() const
        {
            return Front();
        }
        
        ConstReference back() const
        {
            return Back();
        }
        // End STL compatibility functions
        
        
        static ArrayView<T> MakeArrayView(const DynamicArray<T>& array)
        {
            return ArrayView<T>(array.GetTypedData(), array.GetSize());
        }
        
        template<PtrSize count>
        static ArrayView<T> MakeArrayView(const FixedArray<T, count>& array)
        {
            return ArrayView<T>(array.Begin(), count);
        }
        
        template<PtrSize count>
        static ArrayView<T> MakeArrayView(const std::array<T, count>& array)
        {
            return ArrayView<T>(array.data(), count);
        }
        
        static ArrayView<T> MakeArrayView(T* ptr, PtrSize count)
        {
            return ArrayView<T>(ptr, count);
        }
        
        static ArrayView<T> MakeArrayView(T* start, T* end)
        {
            return ArrayView<T>(start, end - start);
        }
    };
}
