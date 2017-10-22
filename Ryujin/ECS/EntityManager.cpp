

#include "EntityManager.hpp"


namespace Ryujin
{
    EntityManager::EntityManager()
    {
        
    }
    
    EntityManager::~EntityManager()
    {
        
    }
    
    EntityManager::EntityManager(const EntityManager& other)
    :
    entities(other.entities)
    {
        
    }
    
    EntityManager& EntityManager::operator=(const EntityManager &other)
    {
        entities = other.entities;
        
        return *this;
    }
    
    Entity EntityManager::Create()
    {
        return entities.Add();
    }
    
    void EntityManager::Destroy(Entity e)
    {
        if (IsEntityAlive(e))
            entities.Remove(e);
    }
    
    bool EntityManager::IsEntityAlive(const Entity& e) const
    {
        return entities.Contains(e);
    }
}