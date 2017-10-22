#pragma once


#include "RyujinCore/OS/Types.hpp"


namespace Ryujin
{
	template<typename Key>
	struct KeyHash;

	template<typename Key>
	struct BasicEquals;

	template<typename Key, typename Value, typename HashFunc, typename EqualsFunc>
	class HashTable;

    class TestCase;
    
    class UnitTestRegistry
    {
    public:
        static HashTable<uint32, TestCase*, KeyHash<uint32>, BasicEquals<uint32>>& GetTable();
        
        static void Register(TestCase* test);
        static void CleanRegistry();
    };
    
    
    class UnitTestRunner
    {
    private:
        uint32 passed;
        uint32 failed;
        
    public:
        UnitTestRunner();
        ~UnitTestRunner();
        
        void RunTests();
        
        FORCEINLINE uint32 GetSuccessfulTestCount() const { return passed; }
        FORCEINLINE uint32 GetFailedTestCount() const { return failed; }
        FORCEINLINE uint32 GetTotalTestsRan() const { return passed + failed; }
    };
}
