


#include "TransformComponentManager.hpp"

namespace Ryujin
{
    const uint32 TransformComponentManager::INVALID_INDEX = NumericLimit<uint32>::Max();
    const TransformComponentManager::Instance TransformComponentManager::INVALID_INSTANCE = { INVALID_INDEX };
    
    TransformComponentManager::TransformComponentManager()
    {
        data.buffer = nullptr;
        data.capacity = 0;
        data.count = 0;
    }
    
    TransformComponentManager::~TransformComponentManager()
    {
        entityToInstanceMap.Clear();
        
        if (data.capacity > 0)
        {
            DefaultEngineAllocator::Instance().Free(data.buffer);
            data.buffer = nullptr;
            data.capacity = 0;
            data.count = 0;
        }
    }
    
    TransformComponentManager::Instance TransformComponentManager::RegisterEntity(const Entity &e)
    {
        if (data.capacity == data.count)
            Allocate(data.capacity * 2 + 8);
        
        uint32 index = data.count;
        data.entities[index] = e;
        data.local[index] = Transform::identity;
        data.world[index] = Transform::identity;
        data.parent[index] = INVALID_INSTANCE;
        data.firstChild[index] = INVALID_INSTANCE;
        data.nextSibling[index] = INVALID_INSTANCE;
        data.prevSibling[index] = INVALID_INSTANCE;
        Instance instance = { data.count++ };
        entityToInstanceMap.Add(e, instance);
        
        return instance;
    }
    
    TransformComponentManager::Instance TransformComponentManager::Lookup(const Entity& e)
    {
        Instance instance = INVALID_INSTANCE;
        entityToInstanceMap.TryGetValue(e, instance);
        
        return instance;
    }
    
    bool TransformComponentManager::Lookup(const Entity& e, Instance& outInstance)
    {
        Instance instance = INVALID_INSTANCE;
        if (entityToInstanceMap.TryGetValue(e, instance))
        {
            outInstance = instance;
            
            return true;
        }
        
        return false;
    }
    
    void TransformComponentManager::Allocate(uint32 numEntries)
    {
        if (numEntries <= data.capacity)
            return;
        
        InstanceData newData;
        
        const PtrSize bytes = numEntries * (sizeof(Entity) + sizeof(Transform) * 2 + sizeof(Instance) * 4);
        newData.buffer = DefaultEngineAllocator::Instance().Allocate(bytes);
        newData.count = data.count;
        newData.capacity = numEntries;
        newData.entities = (Entity*)newData.buffer;
        newData.local = (Transform*)(newData.entities + numEntries);
        newData.world = (newData.local + numEntries);
        newData.parent = (Instance*)(newData.world + numEntries);
        newData.firstChild = (Instance*)(newData.parent + numEntries);
        newData.nextSibling = (Instance*)(newData.firstChild + numEntries);
        newData.prevSibling = (Instance*)(newData.nextSibling + numEntries);
        Memory::MemSet(newData.buffer, 0, bytes);
        
//        uint32 count = data.count > 0 ? newData.capacity - data.count : newData.capacity;
        
        if (data.count > 0)
        {
            Memory::MemCpy(newData.entities, data.entities, sizeof(Entity) * data.count);
            
            Memory::MemCpy(newData.local, data.local, sizeof(Transform) * data.count);
            Memory::MemCpy(newData.world, data.world, sizeof(Transform) * data.count);
            
            Memory::MemCpy(newData.parent, data.parent, sizeof(Instance) * data.count);
            Memory::MemCpy(newData.firstChild, data.firstChild, sizeof(Instance) * data.count);
            Memory::MemCpy(newData.nextSibling, data.nextSibling, sizeof(Instance) * data.count);
            Memory::MemCpy(newData.prevSibling, data.prevSibling, sizeof(Instance) * data.count);
        }
        
        
        if (data.buffer)
            DefaultEngineAllocator::Instance().Free(data.buffer);
        data = newData;
    }
    
    void TransformComponentManager::SetParent(Instance i, Instance parent)
    {
        Instance currentParent = data.parent[i];
        
        if (currentParent.IsValid())
        {
            Instance prevSibling = data.prevSibling[i];
            Instance nextSibling = data.nextSibling[i];
            
            if (data.firstChild[currentParent] == i)
            {
                data.firstChild[currentParent] = nextSibling;
            }
            else
            {
                RYUJIN_ASSERT(prevSibling.instance != INVALID_INSTANCE.instance, "bad previous sibling instance");
                data.nextSibling[prevSibling] = nextSibling;
            }
            
            if (nextSibling.IsValid())
            {
                data.prevSibling[nextSibling] = prevSibling;
            }
        }
        
        data.parent[i] = parent;
        Instance firstChild = data.firstChild[parent];
        
        data.firstChild[parent] = i;
        data.nextSibling[i] = firstChild;
        
        if(firstChild.IsValid())
            data.prevSibling[firstChild] = i;
    }
    
    void TransformComponentManager::SetLocalMatrix(Instance instance, Vector3 pos, Vector3 scale, Quaternion orientation)
    {
        RYUJIN_ASSERT(instance < data.capacity, "Instance index out of bounds");
        
        Transform transform(pos, scale, orientation);
        SetLocalMatrix(instance, transform);
    }
    
    void TransformComponentManager::SetLocalMatrix(Instance instance, const Transform& transform)
    {
        RYUJIN_ASSERT(instance < data.capacity, "Instance index out of bounds");
        
        data.local[instance] = transform;
        Instance parent = data.parent[instance];
        Transform parentTransform = parent.IsValid() ? data.world[parent] : Transform::identity;
        PerformTransform(parentTransform, instance);
    }
    
    void TransformComponentManager::Translate(TransformComponentManager::Instance instance, const Vector3 &position)
    {
        data.local[instance.instance].AddTranslation(position);
        
        PerformTransform(instance);
    }
    
    void TransformComponentManager::Scale(TransformComponentManager::Instance instance, const Vector3 &scale)
    {
        data.local[instance.instance].Scale(scale);
        
        PerformTransform(instance);
    }
    
    void TransformComponentManager::Rotate(TransformComponentManager::Instance instance, const Quaternion &quat)
    {
        data.local[instance.instance].ConcatRotation(quat);
    }
    
    void TransformComponentManager::PerformTransform(Instance instance)
    {
        Instance parent = data.parent[instance.instance];
        Transform transform = parent.IsValid() ? data.world[parent.instance] : Transform::identity;
        
        PerformTransform(transform, instance);
    }
    
    void TransformComponentManager::PerformTransform(Transform& parent, Instance instance)
    {
        data.world[instance] = parent * data.local[instance];
        ModifiedTransform newTransform;
        newTransform.entity = data.entities[instance];
        newTransform.transform = data.world[instance];
        dirtyTransforms.Add(newTransform);
        
        Instance child = data.firstChild[instance];
        while (child.IsValid())
        {
            PerformTransform(data.world[instance], child);
            child = data.nextSibling[child];
        }
    }
    
    void TransformComponentManager::Update(float dt)
    {
    }
    
    Matrix44 TransformComponentManager::GetWorldMatrix(Instance i) const
    {
        RYUJIN_ASSERT(i < data.capacity, "Instance index out of bounds");
        return data.world[i].ToMatrix();
    }
    
    Matrix44 TransformComponentManager::GetLocalMatrix(Instance i) const
    {
        RYUJIN_ASSERT(i < data.capacity, "Instance index out of bounds");
        return data.local[i].ToMatrix();
    }
    
    Transform& TransformComponentManager::GetWorldTransform(Instance i) const
    {
        RYUJIN_ASSERT(i < data.capacity, "Instance index out of bounds");
        return data.world[i];
    }
    
    Transform& TransformComponentManager::GetLocalTransform(Instance i) const
    {
        RYUJIN_ASSERT(i < data.capacity, "Instance index out of bounds");
        return data.local[i];
    }
    
    void TransformComponentManager::Destroy(Instance instance)
    {
        RYUJIN_ASSERT(instance < data.capacity, "Instance index out of bounds");
        
        uint32 lastInstance = data.count == 0 ? 0 : data.count - 1;
        Entity e = data.entities[instance];
        Entity lastEntity = data.entities[lastInstance];
        
        data.local[instance] = data.local[lastInstance];
        data.world[instance] = data.world[lastInstance];
        
        entityToInstanceMap[lastEntity] = instance;
        
        entityToInstanceMap.Remove(e);
        
        --data.count;
    }
}