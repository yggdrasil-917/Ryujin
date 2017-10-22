
#pragma once


#include "RyujinCore/OS/Platform.hpp"
#include "RyujinCore/CoreUtils/StringHash.hpp"

#include "../Utils/ExtensionsBase.hpp"

namespace Ryujin
{
    enum class ResourceState : uint8
    {
        Invalid,
        Loading,
        Valid,
        Failed,
        
        Count
    };
    
    
    class AssetRegistry;
    
    
    class EXTENSIONS_API ResourceBase
    {
    protected:
        StringHash hash;
        ResourceState state;
        AssetRegistry* registry;
        uint64 timeQueuedForDeletion;
        
    public:
        enum { MAX_INACTIVE_DURATION = 30000 }; // 30 seconds in ms
        
        ResourceBase()
        : state(ResourceState::Invalid)
        {
            
        }
        
        ResourceBase(const char* name)
        : hash(name),
          state(ResourceState::Invalid)
        {
            
        }
        
        VIRTUAL ~ResourceBase() {}
        
        FORCEINLINE void SetAssetRegistryOwner(AssetRegistry* owner) { registry = owner; }
        FORCEINLINE void SetTimeQueuedForDeletion(uint64 t) { timeQueuedForDeletion = t; }
        FORCEINLINE void SetState(ResourceState newState) { state = newState; }
        
        FORCEINLINE ResourceState GetResourceState() const { return state; }
        FORCEINLINE uint32 GetUniqueId() const { return hash.GetHash(); }
        FORCEINLINE const char* GetAssetName() const { return hash.ToString(); }
        FORCEINLINE uint64 GetTimeQueuedForDeletion() const { return timeQueuedForDeletion; }
        FORCEINLINE AssetRegistry* GetRegistryOwner() const { return registry; }
    };
}
