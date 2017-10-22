

#include "TagManager.hpp"

//#include "RyujinRenderer/Renderer/RendererCommon.hpp"
//#include "RyujinRenderer/Renderer/Model.hpp"

//#include "../ResourceManagers/ModelResourceManager.hpp"


namespace Ryujin
{
    TagManager::TagManager()
    {
        
    }
    
    TagManager::~TagManager()
    {
        RemoveAllTags();
    }
    
    TagManager::TagManager(const TagManager& other)
    :
    entityToTag(other.entityToTag),
    tagToEntity(other.tagToEntity)
    {
        
    }
    
    TagManager& TagManager::operator=(const TagManager& other)
    {
        entityToTag = other.entityToTag;
        tagToEntity = other.tagToEntity;
        
        return *this;
    }
    
    void TagManager::SetTag(const Entity& entity, String name)
    {
        String outValue = nullptr;
        if (!entityToTag.TryGetValue(entity, outValue))
        {
            entityToTag.Add(entity, name);
            tagToEntity.Add(name, entity);
        }
    }
    
    void TagManager::RemoveTag(const char *tag)
    {
        Entity e;
        if (tagToEntity.TryGetValue(tag, e))
        {
            tagToEntity.Remove(tag);
            entityToTag.Remove(e);
        }
    }
    
    bool TagManager::IsRegistered(const char *tag) const
    {
        Entity e;
        if (tagToEntity.TryGetValue(tag, e))
        {
            return true;
        }
        
        return false;
    }
    
    const char* TagManager::GetTag(const Entity& entity) const
    {
        String outValue = nullptr;
        if (!entityToTag.TryGetValue(entity, outValue))
        {
            return "";
        }
        
        return outValue;
    }
    
    Entity TagManager::GetEntity(const char *tag) const
    {
        Entity e;
        if (tagToEntity.TryGetValue(tag, e))
        {
            return e;
        }
        
        return CreateInvalidHandle();
    }
    
    void TagManager::GC(const EntityManager& em)
    {
        HashTable<Entity, String, EntityHash>::Iterator it(entityToTag);
        while (it)
        {
            Entity& e = it->key;
            if (!em.IsEntityAlive(e))
            {
                entityToTag.Remove(e);
            }
            ++it;
        }
    }
    
    void TagManager::RemoveAllTags()
    {
        tagToEntity.Clear();
        entityToTag.Clear();
    }
}
