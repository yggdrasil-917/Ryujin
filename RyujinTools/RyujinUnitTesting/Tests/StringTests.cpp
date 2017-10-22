

#include "../RyujinUnitTesting/RyujinUnitTesting.hpp"
#include "../RyujinUnitTesting/TestCase.hpp"

#include "RyujinCore/Containers/String.hpp"

namespace Ryujin
{
    TEST_CASE(StringConstructionTest, "Test for various ctors")
    {
        String c1;
        REQUIRE(c1 == "");
        
        String c2("test");
        REQUIRE(!c2.IsHeapAllocated() && c2 == "test");
        
        String c3(c2);
        REQUIRE(!c3.IsHeapAllocated() && c2 == c3 && c3 == "test");
        
        String c33 = c3;
        REQUIRE(!c33.IsHeapAllocated() && c2 == c33 && c33 == "test");
        
        String c4(Move(c2));
        REQUIRE(!c4.IsHeapAllocated() && c4 == "test" && c2.Length() == 0);
        
        String c44 = Move(c4);
        REQUIRE(!c44.IsHeapAllocated() && c44 == "test" && c4.Length() == 0);
    }
    
    TEST_CASE(StringStartsWithTest, "Test for starts with check")
    {
        String test("TestForStringStartsWith");
        REQUIRE(test.StartsWith("TestForString"));
        REQUIRE(!test.StartsWith("blahblah"));
    }
    
    TEST_CASE(StringEndsWithTest, "Test for ends with check")
    {
        String test("TestForStringEndsWith");
        REQUIRE(test.EndsWith("EndsWith"));
        REQUIRE(!test.EndsWith("blahblah"));
    }
    
    TEST_CASE(StringHeapAllocatedTest, "Test for buffer optimization")
    {
        String test;
        for (int32 i = 0; i < SMALL_STRING_LENGTH - 1; ++i)
        {
            test += "a";
        }
        
        REQUIRE(!test.IsHeapAllocated());
        
        test += "a";
        REQUIRE(test.IsHeapAllocated());
        
        test = "abc";
        REQUIRE(!test.IsHeapAllocated());
    }
    
    TEST_CASE(StringConcatTest, "Test for concatenation")
    {
        String test1("test1");
        String test2("test2");
        String concat = String::Concat(*test1, *test2);
        REQUIRE(concat == "test1test2");
        
        String concat2 = test1;
        concat2 += test2;
        REQUIRE(concat2 == "test1test2");
        
        String concat3 = test1;
        concat3 += "test2";
        REQUIRE(concat3 == "test1test2");
        
        String concat4 = test1 + "test2";
        REQUIRE(concat4 == "test1test2");
    }
    
    TEST_CASE(StringFindReplaceTest, "Test for finding substring and string replacement")
    {
        String str("This is a test");
        int32 index = str.Find("a test");
        REQUIRE(index == 8);
        
        str.Replace(index, 12, "another test");
        REQUIRE(str == "This is another test");
    }
    
    TEST_CASE(StringToLower, "To lower test")
    {
        String s("STRING TO LOWER");
        String lower = s.ToLower();
        REQUIRE(lower == "string to lower");
    }
    
    TEST_CASE(StringToUpper, "To upper test")
    {
        String s("string to upper");
        String upper = s.ToUpper();
        REQUIRE(upper == "STRING TO UPPER");
    }
}
