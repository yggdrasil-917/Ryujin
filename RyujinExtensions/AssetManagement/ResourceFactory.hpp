
#pragma once

#include "RyujinCore/Containers/HashTable.hpp"

#include "Resource.hpp"


namespace Ryujin
{
#define DECLARE_RESOURCE_FACTORY_REGISTRANT(classname) \
    private: \
        static const CreatorImpl<classname> creator; \
        static const uint32 factoryId; \
    public: \
        static uint32 GetFactoryId() { return factoryId; }
    
#define IMPLEMENT_RESOURCE_FACTORY_REGISTRANT(classname) \
    const CreatorImpl<classname> classname::creator(#classname); \
    const uint32 classname::factoryId = (uint32)CalculateFNV(#classname);
    
    
    class EXTENSIONS_API Creator
    {
    public:
        Creator(const char* className);
        
        VIRTUAL ResourceBase* Create(const char* resourceName, void* setup) = 0;
    };
    
    template<class T>
    class EXTENSIONS_API CreatorImpl : public Creator
    {
    public:
        CreatorImpl(const char* className)
        : Creator(className)
        {
            
        }
        
        ResourceBase* Create(const char* resourceName, void* resourceDesc) OVERRIDE FINAL
        {
            return RYUJIN_NEW T(resourceName, resourceDesc);
        }
    };
    
    class EXTENSIONS_API ResourceFactory
    {
    public:
        static ResourceBase* Create(const uint32 classId, const char* resourceName, void* setupData);
        static void Register(const uint32 classId, Creator* creator);
        
    private:
        static HashTable<uint32, Creator*>& GetTable();
    };
}
