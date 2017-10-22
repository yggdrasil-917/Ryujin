

#include "../RyujinUnitTesting/RyujinUnitTesting.hpp"
#include "../RyujinUnitTesting/TestCase.hpp"

#include "RyujinCore/Containers/HashTable.hpp"



namespace Ryujin
{
    TEST_CASE(HashTableAddRemoveTest, "Test for adding and removing elements")
    {
        HashTable<int32, int32> table;
        int32 key = 0;
        table.Add(key, 1);
        int32 out = 0;
        REQUIRE(table.Count() == 1 && table.TryGetValue(key, out) && out == 1);
        
        table.Remove(key);
        REQUIRE(table.Count() == 0 && !table.TryGetValue(key, out));
        
        int32 key2 = 1;
        table[key2] = 20;
        REQUIRE(table.Count() == 1 && table.TryGetValue(key2, out) && out == 20);
    }
    
    TEST_CASE(HashTableAddRemoveObjectTest, "Test for adding and removing objects")
    {
        HashTable<uint32, StringHash> table;
        StringHash hash("test");
        
        table.Add(hash.GetHash(), hash);
        StringHash hash2 = table[hash.GetHash()];
        REQUIRE(hash2.GetHash() == hash.GetHash());
        String str(hash.ToString());
        String str2(hash2.ToString());
        REQUIRE(str == str2);
        
        table.Clear();
        REQUIRE(table.Count() == 0);
    }
    
    TEST_CASE(HashTableNestedTableTestbl, "Test for storing a hash table inside another hash table")
    {
        StringMap<HashTable<String, int32, StrHash, StrEquals>> mainTable;
        StringMap<int32> nestedTable;
        nestedTable.Add("one", 1);
        mainTable.Add("nested", nestedTable);
        
        StringMap<int32> table;
        REQUIRE(mainTable.TryGetValue("nested", table));

        int32 one = 0;
        REQUIRE(table.TryGetValue("one", one));
        REQUIRE(one == 1);
    }
}
