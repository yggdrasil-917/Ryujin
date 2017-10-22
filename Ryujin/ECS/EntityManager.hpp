
#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP


#include "ComponentManagerCommon.hpp"


namespace Ryujin
{
    
    class EntityManager
    {
    private:
        EntityArray entities;
        
    public:
        EntityManager();
        ~EntityManager();
        
        EntityManager(const EntityManager& other);
        EntityManager& operator=(const EntityManager& other);
        
        Entity Create();
        void Destroy(Entity e);
        bool IsEntityAlive(const Entity& entity) const;
        FORCEINLINE const EntityArray& GetEntityArray() const { return entities; }
        FORCEINLINE uint32 GetEntityCount() const { return entities.GetCount(); }
    };
}


#endif