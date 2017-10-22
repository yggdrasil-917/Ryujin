
#ifndef TAG_MANAGER_HPP
#define TAG_MANAGER_HPP

#include "EntityManager.hpp"


namespace Ryujin
{
    class TagManager
    {
    private:
        HashTable<Entity, String, EntityHash> entityToTag;
        HashTable<String, Entity, StrHash> tagToEntity;
        
    public:
        TagManager();
        ~TagManager();
        
        TagManager(const TagManager& other);
        TagManager& operator=(const TagManager& other);
        
        void SetTag(const Entity& entity, String name);
        void RemoveTag(const char* tag);
        
        bool IsRegistered(const char* tag) const;
        const char* GetTag(const Entity& entity) const;
        Entity GetEntity(const char* tag) const;
        
        void GC(const EntityManager& em);
        void RemoveAllTags();
    };
}


#endif