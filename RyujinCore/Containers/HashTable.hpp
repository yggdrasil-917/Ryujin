#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP


#include "../Logger/Logger.hpp"
#include "../Allocators/Memory.hpp"

#include "DynamicArray.hpp"


namespace Ryujin
{
	// function prototypes for hashing primitive types such as int, uint32, const char*
	FORCEINLINE uint32 Hash(int key);
	FORCEINLINE uint32 Hash(uint32 key);
	FORCEINLINE uint32 Hash(const char * key);
    
    template<typename Key>
    struct KeyHash
    {
        uint64 operator ()(const Key& key, uint32 tableSize) const
        {
            return static_cast<uint32>(key) % tableSize;
        }
    };
    
    struct StrHash
    {
        uint64 operator ()(const char* key, uint32 tableSize) const
        {
            return Hash(key) % tableSize;
        }
    };
    
    template<typename Key>
    struct BasicEquals
    {
        bool operator()(const Key& a, const Key& b) const
        {
            return Memory::MemCmp(&a, &b, sizeof(Key)) == 0;
        }
    };
    
    struct StrEquals
    {
        bool operator()(const char* a, const char* b) const
        {
            return std::strcmp(a, b) == 0;
        }
    };
    
    template<typename Key, typename Value, typename HashFunc = KeyHash<Key>, typename EqualsFunc = BasicEquals<Key>>
    class HashTable
    {
    public:
        struct HashEntry
        {
            Key key;
            Value value;
            HashEntry* next;
            HashEntry* prev;
            
            HashEntry(const Key& inKey, const Value& inValue)
            :
            key(inKey),
            value(inValue),
            next(nullptr),
            prev(nullptr)
            {}
            
            ~HashEntry()
            {
                next = nullptr;
                prev = nullptr;
            }
        };
        
    private:
        HashFunc hashFunc;
        EqualsFunc equalTo;
        HashEntry** table;
        uint32 tableCapacity;
        uint32 itemCount;
        
    public:
        HashTable()
        {
            itemCount = 0;
            tableCapacity = 128;
            table = RYUJIN_NEW HashEntry*[128];
            Memory::MemSet(table, 0, sizeof(HashEntry*) * 128);
        }
        
        HashTable(uint32 capacity)
        {
            itemCount = 0;
            tableCapacity = capacity;
            table = RYUJIN_NEW HashEntry*[capacity];
            Memory::MemSet(table, 0, sizeof(HashEntry*) * capacity);
        }
        
        HashTable(const HashTable<Key, Value, HashFunc, EqualsFunc>& other)
        {
            itemCount = 0;//other.itemCount;
            tableCapacity = other.tableCapacity;
            table = RYUJIN_NEW HashEntry*[tableCapacity];
            Memory::MemSet(table, 0, sizeof(HashEntry*) * tableCapacity);
            
            for (uint32 i = 0; i < tableCapacity; ++i)
            {
                HashEntry* item = other.table[i];
                while (item)
                {
                    Add(item->key, item->value);
                    item = item->next;
                }
            }
        }
        
        HashTable<Key, Value, HashFunc, EqualsFunc>& operator=(const HashTable<Key, Value, HashFunc, EqualsFunc>& other)
        {
            itemCount = 0;//other.itemCount;
            tableCapacity = other.tableCapacity;
            table = RYUJIN_NEW HashEntry*[tableCapacity];
            Memory::MemSet(table, 0, sizeof(HashEntry*) * tableCapacity);
            
            for (uint32 i = 0; i < tableCapacity; ++i)
            {
                HashEntry* item = other.table[i];
                while (item)
                {
                    Add(item->key, item->value);
                    item = item->next;
                }
            }
            
            return *this;
        }
        
        HashTable(HashTable<Key, Value, HashFunc, EqualsFunc>&& other)
        {
            itemCount = Move(other.itemCount);
            tableCapacity = Move(other.tableCapacity);
            table = Move(other.table);
            
            other.table = nullptr;
            other.tableCapacity = 0;
            other.itemCount = 0;
        }
        
        HashTable<Key, Value, HashFunc, EqualsFunc>& operator=(HashTable<Key, Value, HashFunc, EqualsFunc>&& other)
        {
            itemCount = Move(other.itemCount);
            tableCapacity = Move(other.tableCapacity);
            table = Move(other.table);
            
            other.table = nullptr;
            other.tableCapacity = 0;
            other.itemCount = 0;
            
            return *this;
        }
        
        ~HashTable()
        {
            Clear();
        }
        
        bool TryGetValue(const Key& key, Value& outValue) const
        {
            uint64 hash = hashFunc(key, tableCapacity);
            HashEntry* item = table[hash];
            while (item)
            {
                if (equalTo(item->key, key))
                {
                    outValue = item->value;
                    return true;
                }
                item = item->next;
            }
            
            return false;
        }
        
        void Add(const Key& key, const Value& value)
        {
            uint64 hash = hashFunc(key, tableCapacity);
            HashEntry* item = table[hash];
            
            while(item && item->next)
            {
                if (equalTo(item->key, key))
                {
                    return;
                }
                item = item->next;
            }
            
            HashEntry* entry = RYUJIN_NEW HashEntry(key, value);
            ++itemCount;
            
            if (item == nullptr)
            {
                table[hash] = entry;
            }
            else
            {
                item->next = entry;
                entry->prev = item;
            }
        }
        
        void Remove(const Key& key)
        {
            uint64 hash = hashFunc(key, tableCapacity);
            HashEntry* item = table[hash];
            
            while(item)
            {
                if (equalTo(item->key, key))
                {
                    break;
                }
                item = item->next;
            }
            
            if (item)
            {
                if (item->prev == nullptr)
                {
                    table[hash] = item->next;
                }
                else
                {
                    item->prev->next = item->next;
                    item->next->prev = item->prev;
                }
                
                delete item;
                item = nullptr;
                --itemCount;
            }
        }
        
        FORCEINLINE uint32 Count() const { return itemCount; }
        FORCEINLINE uint32 GetCapacity() const { return tableCapacity; }
        
        void Clear()
        {
            if (tableCapacity == 0)
                return;
            
            for (uint32 i = 0; i < tableCapacity; ++i)
            {
                HashEntry* item = table[i];
                while (item)
                {
                    HashEntry* temp = item;
                    item = item->next;
                    delete temp;
                    temp = nullptr;
                }
                table[i] = nullptr;
            }
            
            tableCapacity = 0;
            itemCount = 0;
            if (table)
                delete[] table;
            table = nullptr;
        }
        
        Value& operator[](const Key& key)
        {
            uint64 hash = hashFunc(key, tableCapacity);
            HashEntry* item = table[hash];
            
            while (item)
            {
                if (item->key == key)
                {
                    return item->value;
                }
                item = item->next;
            }
            
            // Add if not found
            Add(key, Value());
            
            // Search for the value again
            item = table[hash];
            
            while (item)
            {
                if (item->key == key)
                {
                    return item->value;
                }
                item = item->next;
            }
            
            RYUJIN_ASSERT(0, "Failed to add new entry");
            return item->value;
        }
        
        
        class Iterator
        {
            friend class HashTable<Key, Value, HashFunc, EqualsFunc>;
        private:
            HashEntry* item;
            const HashTable<Key, Value, HashFunc, EqualsFunc>* tableRef;
            uint32 currentBucket;
            
        public:
            Iterator(const HashTable<Key, Value, HashFunc, EqualsFunc>& table)
            {
                tableRef = &table;
                item = table.table[0];
                uint32 capacity = table.tableCapacity;
                uint32 i = 0;
                while (!item && i < capacity)
                {
                    item = table.table[i++];
                }
                currentBucket = i;
            }
            
            
            FORCEINLINE Iterator& operator++()
            {
                if (item)
                {
                    item = item->next;
                }
                
                if (!item)
                {
                    while (!item && currentBucket < tableRef->tableCapacity)
                    {
                        item = tableRef->table[currentBucket];
                        ++currentBucket;
                    }
                }
                
                return *this;
            }
            
            
            FORCEINLINE bool operator==(const Iterator& it) const { return item == it.item; }
            FORCEINLINE bool operator!=(const Iterator& it) const { return item != it.item; }
            
            FORCEINLINE HashEntry& operator*() const { return *item; }
            FORCEINLINE HashEntry* operator->() const { return item; }
            FORCEINLINE operator bool() const { return item != nullptr; }
        };
    };
    
    template<typename Value>
    using StringMap = HashTable<class String, Value, StrHash, StrEquals>;
    
	// Function definitions for hashing int, uint32, const char*
  // http://www.azillionmonkeys.com/qed/hash.html
#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
	|| defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16 *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32)(((const uint8 *)(d))[1])) << 8)\
	+ (uint32)(((const uint8 *)(d))[0]))
#endif

	FORCEINLINE uint32 Hash(int32 key)
	{
		key = (int32)((uint32)key + 0x7ed55d16) + ((uint32)key << 12);
		key = (int32)((uint32)key ^ 0xc761c23c) ^ ((uint32)key >> 19);
		key = (int32)((uint32)key + 0x165667b1) + ((uint32)key << 5);
		key = (int32)((uint32)key + 0xd3a2646c) ^ ((uint32)key << 9);
		key = (int32)((uint32)key + 0xfd7046c5) + ((uint32)key << 3);
		key = (int32)((uint32)key ^ 0xb55a4f09) ^ ((uint32)key >> 16);
		return key;
	}

	FORCEINLINE uint32 Hash(uint32 key)
	{
		key = (key + 0x7ed55d16) + (key << 12);
		key = (key ^ 0xc761c23c) ^ (key >> 19);
		key = (key + 0x165667b1) + (key << 5);
		key = (key + 0xd3a2646c) ^ (key << 9);
		key = (key + 0xfd7046c5) + (key << 3);
		key = (key ^ 0xb55a4f09) ^ (key >> 16);
		return key;
	}

	FORCEINLINE int32 StrLen(const char * str)
	{
		if (!str)
			return 0;

		int count = 0;

		const char* it = str;
		while (*it)
		{
			++count;
			++it;
		}

		return count;
	}

	FORCEINLINE uint32 Hash(const char * key)
	{
		int32 len = StrLen(key);
		uint32 hash = len;
		uint32 tmp;
		int32 rem;

		if (len <= 0 || key == NULL)
            return 0;

		rem = len & 3;
		len >>= 2;

		/* Main loop */
		for (; len > 0; len--) 
		{
			hash += get16bits(key);
			tmp = (get16bits(key + 2) << 11) ^ hash;
			hash = (hash << 16) ^ tmp;
			key += 2 * sizeof (uint16);
			hash += hash >> 11;
		}

		/* Handle end cases */
		switch (rem) 
		{
		case 3:
            hash += get16bits(key);
			hash ^= hash << 16;
			hash ^= key[sizeof (uint16)] << 18;
			hash += hash >> 11;
			break;
		case 2:
            hash += get16bits(key);
			hash ^= hash << 11;
			hash += hash >> 17;
			break;
		case 1:
            hash += *key;
			hash ^= hash << 10;
			hash += hash >> 1;
		}

		/* Force "avalanching" of final 127 bits */
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;

		return hash;
	}
}


#endif
