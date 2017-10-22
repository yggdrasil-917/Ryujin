

#include "../RyujinUnitTesting/RyujinUnitTesting.hpp"
#include "../RyujinUnitTesting/TestCase.hpp"
#include "RyujinCore/Containers/DynamicArray.hpp"

namespace Ryujin
{
    TEST_CASE(DynamicArraySizeTest, "Dynamic array size test")
    {
        DynamicArray<int32> arr = { 1, 2, 3 };
        REQUIRE(arr.GetSize() == 3);
        
        arr.Resize(20);
        for (uint32 i = 0; i < arr.GetSize(); ++i)
        {
            REQUIRE(arr.Contains(i + 1));
        }
        
        arr.Remove(0);
        REQUIRE(arr.GetSize() == 2);
        for (uint32 i = 0; i < arr.GetSize(); ++i)
        {
            REQUIRE(arr.Contains(i + 2));
        }
        
        arr.Add(10);
        REQUIRE(arr.GetSize() == 3);
        
        arr.RemoveAll();
        REQUIRE(arr.GetSize() == 0);
    }
    
    TEST_CASE(DynamicArrayMoveTest, "Testing move op on dynamic array")
    {
        DynamicArray<int32> arr = { 1, 2, 3 };
        DynamicArray<int32> moveResult = Move(arr);
        REQUIRE(moveResult.GetSize() == 3 && arr.GetSize() == 0 && arr.GetData() == nullptr);
        
        DynamicArray<int32> moveCtor(Move(moveResult));
        for (uint32 i = 0; i < moveCtor.GetSize(); ++i)
        {
            REQUIRE(moveCtor.Contains(i + 1));
        }
        REQUIRE(moveCtor.GetSize() == 3 && moveResult.GetSize() == 0 && moveResult.GetData() == nullptr);
    }
    
    TEST_CASE(DynamicArrayCopyTest, "Test for copy op")
    {
        DynamicArray<int32> a = { 1, 2, 3 };
        DynamicArray<int32> b;
        b.Copy(a.GetTypedData(), a.GetSize());
        DynamicArray<int32> c(a);
        REQUIRE(a.GetSize() == b.GetSize());
        REQUIRE(a.GetSize() == c.GetSize());
        REQUIRE(c.GetSize() == b.GetSize());
        for(uint32 i = 0; i < a.GetSize(); ++i)
        {
            REQUIRE(a[i] == b[i]);
            REQUIRE(a[i] == c[i]);
            REQUIRE(a[i] == b[i]);
        }
    }
    
    TEST_CASE(DynamicArraySortTest, "Testing std sort")
    {
        DynamicArray<int32> arr = { 6, 5, 4, 3, 2, 1 };
        std::sort(arr.begin(), arr.end());
        int32 sorted[] = { 1, 2, 3, 4, 5, 6 };
        for (int32 i = 0; i < countof(sorted); ++i)
        {
            REQUIRE(sorted[i] == arr[i]);
        }
    }
    
    TEST_CASE(DynamicArrayRemoveSlowTest, "Test for RemoveSlow")
    {
        DynamicArray<int32> a = { 1, 2, 3 };
        DynamicArray<int32> b = { 2, 3 };
        a.RemoveSlow(0);
        REQUIRE(a.GetSize() == b.GetSize());
        for (uint32 i = 0; i < a.GetSize(); ++i)
        {
            REQUIRE(a[i] == b[i]);
        }
    }
}
