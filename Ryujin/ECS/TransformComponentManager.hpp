
#ifndef TRANSFORM_COMPONENT_MANAGER_HPP
#define TRANSFORM_COMPONENT_MANAGER_HPP


#include "ComponentManagerCommon.hpp"
#include "RyujinCore/Math/Transform.hpp"

namespace Ryujin
{
    
    struct ModifiedTransform
    {
        Entity entity;
        Transform transform;
    };
    
    
    class TransformComponentManager
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
            
            Entity* entities;
            Transform* local;
            Transform* world;
            Instance* parent;
            Instance* firstChild;
            Instance* nextSibling;
            Instance* prevSibling;
        };
        InstanceData data;
        
        HashTable<Entity, Instance, EntityHash> entityToInstanceMap;
        DynamicArray<ModifiedTransform> dirtyTransforms;
        
        void PerformTransform(Transform& parent, Instance instance);
        void PerformTransform(Instance instance);
        
    public:
        TransformComponentManager();
        ~TransformComponentManager();
        
        Instance RegisterEntity(const Entity& e);
        Instance Lookup(const Entity& e);
        bool Lookup(const Entity& e, Instance& outInstance);
        void SetParent(Instance i, Instance parent);
        void Allocate(uint32 numEntries);
        void Destroy(Instance instance);
        void SetLocalMatrix(Instance instance, Vector3 pos, Vector3 scale, Quaternion orientation);
        void SetLocalMatrix(Instance instance, const Transform& transform);
        void Translate(Instance instance, const Vector3& position);
        void Scale(Instance instance, const Vector3& scale);
        void Rotate(Instance instance, const Quaternion& quat);
        void Update(float dt);
        
        Matrix44 GetWorldMatrix(Instance i) const;
        Matrix44 GetLocalMatrix(Instance i) const;
        Transform& GetWorldTransform(Instance i) const;
        Transform& GetLocalTransform(Instance i) const;
        
        FORCEINLINE void ClearModifiedTransforms() { dirtyTransforms.RemoveAll(); }
        FORCEINLINE const DynamicArray<ModifiedTransform>& GetModifiedTransforms() const { return dirtyTransforms; }
    };
}



#endif