#include "RyujinUnitTesting.hpp"

#include "RyujinCore/Containers/String.hpp"
#include "RyujinCore/Containers/HashTable.hpp"
#include "TestCase.hpp"

namespace Ryujin
{
    HashTable<uint32, TestCase*>& UnitTestRegistry::GetTable()
    {
        static HashTable<uint32, TestCase*> tests;
        return tests;
    }
    
    void UnitTestRegistry::Register(TestCase* test)
    {
        GetTable().Add(test->hash.GetHash(), test);
    }
    
    void UnitTestRegistry::CleanRegistry()
    {
        HashTable<uint32, TestCase*>& testMap = UnitTestRegistry::GetTable();
        
        for (HashTable<uint32, TestCase*>::Iterator it(testMap); it; ++it)
        {
            SAFE_DELETE(it->value);
        }
        
        testMap.Clear();
    }
    
    UnitTestRunner::UnitTestRunner()
    : passed(0),
      failed(0)
    {
        
    }
    
    UnitTestRunner::~UnitTestRunner()
    {
        
    }
    
    void UnitTestRunner::RunTests()
    {
        HashTable<uint32, TestCase*>& testMap = UnitTestRegistry::GetTable();
        
        for (HashTable<uint32, TestCase*>::Iterator it(testMap); it; ++it)
        {
            it->value->Invoke();
            if (it->value->test->HasFailed())
            {
                ++failed;
            }
            else
            {
                ++passed;
            }
        }
        
        Debugf(Info, "Ran %u tests", failed + passed);
        Debugf(Info, "\t%u failed", failed);
        Debugf(Info, "\t%u passed", passed);
    }
}
