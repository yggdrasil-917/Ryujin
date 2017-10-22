

#include "ResourceFactory.hpp"


namespace Ryujin
{
    Creator::Creator(const char* className)
    {
        ResourceFactory::Register((uint32)CalculateFNV(className), this);
    }
    
    ResourceBase* ResourceFactory::Create(const uint32 classId, const char* resourceName, void* setupData)
    {
        Creator* creator = nullptr;
        if (GetTable().TryGetValue(classId, creator))
        {
            return creator->Create(resourceName, setupData);
        }
        
        return nullptr;
    }
    
    void ResourceFactory::Register(const uint32 classId, Creator *creator)
    {
        GetTable()[classId] = creator;
    }
    
    HashTable<uint32, Creator*>& ResourceFactory::GetTable()
    {
        static HashTable<uint32, Creator*> table(1024);
        return table;
    }
}
