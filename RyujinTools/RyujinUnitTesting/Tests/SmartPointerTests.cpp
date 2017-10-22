

#include "../RyujinUnitTesting/RyujinUnitTesting.hpp"
#include "../RyujinUnitTesting/TestCase.hpp"

#include "RyujinCore/CoreUtils/SmartPointer.hpp"

namespace Ryujin
{
    TEST_CASE(UniquePtrConstructionTest, "Test for constructing unique pointer")
    {
        const int32 value = 10;
        UniquePtr<int32> p(RYUJIN_NEW int32(value));
        REQUIRE(p);
        
        UniquePtr<int32> p2;
        REQUIRE(!p2);
        
        UniquePtr<int32> p3(Move(p));
        REQUIRE(p3 && !p);
        REQUIRE(*p3 == value);
        
        UniquePtr<int32> p4 = Move(p3);
        REQUIRE(p4 && !p3);
        REQUIRE(*p4 == value);
    }
    
    TEST_CASE(UniquePtrDestructionTest, "Test for destroying unique pointer")
    {
        const int32 value = 10;
        UniquePtr<int32> p(RYUJIN_NEW int32(value));
        
        int32* ptr = p.Release();
        REQUIRE(!p && ptr);
        REQUIRE(*ptr == value);
        
        SAFE_DELETE(ptr);
    }
    
    TEST_CASE(SharedPtrConstructionTest, "Test for constructing shared pointer")
    {
        const int32 value = 10;
        SharedPtr<int32> p(RYUJIN_NEW int32(value));
        REQUIRE(p);
        
        SharedPtr<int32> p2(p);
        REQUIRE(p2 && p);
        {
            SharedPtr<int32> p22 = p2;
            REQUIRE(p22 && *p22 == value);
        }
        
        UniquePtr<int32> up(RYUJIN_NEW int32(20));
        SharedPtr<int32> p3(Move(up));
        REQUIRE(!up && p3);
        REQUIRE(*p3 == 20);
        
        p3 = nullptr;
        REQUIRE(p3 == nullptr);
    }
    
    TEST_CASE(WeakPtrConstructionTest, "Test for constructing weak pointer")
    {
        WeakPtr<int32> p;
        REQUIRE(p.Expired());
        
        {
            SharedPtr<int32> sp(RYUJIN_NEW int32(1));
            p = sp;
            REQUIRE(!p.Expired());
            
            SharedPtr<int32> locked = p.Lock();
            REQUIRE(locked && *locked == 1);
        }
        
        REQUIRE(p.Expired());
    }
}
