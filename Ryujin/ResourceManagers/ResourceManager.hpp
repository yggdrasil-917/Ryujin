
#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP


#include "ResourceTypes.hpp"
#include "RyujinCore/Containers/HashTable.hpp"
#include "RyujinCore/Containers/SparseArray.hpp"

#include "RyujinCore/Threading/Thread.hpp"


namespace Ryujin
{
    class AssetHandle
    {
    private:
        AtomicVar<int32> refCount;
        
    public:
        ResourceHandle handle;
        
        AssetHandle()
        : refCount(1)
        {
            handle.index = 0;
            handle.generation = 1;
        }
        
        AssetHandle(uint32 index)
        : refCount(1)
        {
            handle.index = index;
            handle.generation = 1;
        }
        
        AssetHandle(ResourceHandle other)
        : refCount(1)
        {
            handle = other;
        }
        
        AssetHandle(const AssetHandle& other)
        {
            refCount = other.refCount.GetValue(Threading::MemoryOrder::MO_Relaxed);
            handle = other.handle;
        }
        
        AssetHandle& operator=(const AssetHandle& other)
        {
            refCount = other.refCount.GetValue(Threading::MemoryOrder::MO_Relaxed);
            handle = other.handle;
            
            return *this;
        }
        
        FORCEINLINE void IncRef() { ++refCount; }
        FORCEINLINE void DecRef() { --refCount; }
        FORCEINLINE int32 GetRefCount() const { return refCount.GetValue(); }
    };
    
    
    template<class T>
    class ResourceManager
    {
    protected:
        String path;
        SparseArray<UniquePtr<T>, ResourceHandle> resources;
        HashTable<uint32, AssetHandle> hashToResourceHandle;
        HashTable<uint32, StringHash> handleToHash;
        AssetHandle defaultHandle;
        Mutex resourceMutex;
        
        VIRTUAL void LoadDefaultAsset() = 0;
        VIRTUAL UniquePtr<T> Load(const String& filename) = 0;
        VIRTUAL void Unload(AssetHandle inHandle) = 0;
        
    public:
        ResourceManager() {}
        VIRTUAL ~ResourceManager() {}
        
        void Initialize(const String& inPath)
        {
            path = inPath;
            LoadDefaultAsset();
        }
        
        AssetHandle Get(const String& filename)
        {
            String file(path + filename);
            StringHash hash(*file);
            
            ScopedLock lock(resourceMutex);
            
            AssetHandle resourceHandle;
            if (hashToResourceHandle.TryGetValue(hash.GetHash(), resourceHandle))
            {
                resourceHandle.IncRef();
                return resourceHandle;
            }
            
            // Add new resource
            UniquePtr<T> resource = Load(file);
            if (resource)
            {
                AssetHandle newHandle(resources.Add());
                resources.MoveObject(newHandle.handle, Move(resource));
                hashToResourceHandle.Add(hash.GetHash(), newHandle);
                handleToHash.Add(newHandle.handle.index, hash);
                
                return newHandle;
            }
            
            // Each manager will need to have a safe default object already loaded
            return defaultHandle;
        }
        
        FORCEINLINE const T* GetResource(AssetHandle inHandle) const
        {
            RYUJIN_ASSERT(resources.Contains(inHandle.handle), "Tried to get resource with invalid handle");
            return resources.GetObject(inHandle.handle).Get();
        }
        
        void Remove(AssetHandle& inHandle)
        {
            if (inHandle.GetRefCount() == 0)
                return;
            
            inHandle.DecRef();
            ScopedLock lock(resourceMutex);
            
            if (inHandle.GetRefCount() == 0 && resources.Contains(inHandle.handle))
            {
                Unload(inHandle.handle);
                resources.Remove(inHandle.handle);
                StringHash hash;
                if (handleToHash.TryGetValue(inHandle.handle.index, hash))
                {
                    hashToResourceHandle.Remove(hash.GetHash());
                }
                handleToHash.Remove(inHandle.handle.index);
            }
        }
    };
}



#endif
