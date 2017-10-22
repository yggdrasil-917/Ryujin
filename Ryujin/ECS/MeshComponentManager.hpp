
#ifndef MESH_COMPONENT_MANAGER_HPP
#define MESH_COMPONENT_MANAGER_HPP


#include "TransformComponentManager.hpp"

#include "../ResourceManagers/ResourceManager.hpp"

namespace Ryujin
{
    
    struct MeshComponent_old
    {
        AssetHandle model;
        Transform transform;
    };
    
    class MeshComponentManager
    {
    public:
        struct Instance
        {
            uint32 instance;
            
            operator uint32() { return instance; }
            
            Instance()
            :
            instance(INVALID_INDEX)
            {}
            
            Instance(uint32 i)
            :
            instance(i)
            {}
            
            Instance(const Instance& other)
            :
            instance(other.instance)
            {}
            
            Instance& operator=(const Instance& other)
            {
                instance = other.instance;
                return *this;
            }
            
            bool IsValid() const
            {
                return instance != INVALID_INDEX;
            }
        };
        
    private:
        static const uint32 INVALID_INDEX;
        static const Instance INVALID_INSTANCE;
        
        struct InstanceData
        {
            uint32 count;
            uint32 capacity;
            void* buffer;
            
            MeshComponent_old* components;
            Entity* entity;
            AABB* bbox;
        };
        InstanceData data;
        
    public:
        MeshComponentManager::Instance RegisterEntity(const Entity& e);
        MeshComponentManager::Instance Lookup(const Entity& e);
        void Allocate(uint32 numEntries);
        
        void Update(float dt);
    };
}



#endif
