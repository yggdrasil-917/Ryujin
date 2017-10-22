#pragma once


#include "RyujinCore/Containers/DynamicArray.hpp"

#include "Component.hpp"



namespace Ryujin
{
    class EXTENSIONS_API RyujinEntity
    {
    private:
        DynamicArray<Component*> components;
        
    public:
        RyujinEntity() {}
        ~RyujinEntity();
        
        uint32 AddComponent(Component* component);
        
        template <class TComponent>
        TComponent* GetComponent(uint32 index) const
        {
            if (index >= 0 && index < components.GetSize())
            {
                return static_cast<TComponent*>(components[index]);
            }
            return nullptr;
        }
        
        template <class TComponent>
        TComponent* GetComponent() const
        {
            for (auto it : components)
            {
                if (it->GetTypeId() == TComponent::GetComponentTypeId())
                {
                    return static_cast<TComponent*>(it);
                }
            }
            return nullptr;
        }
        
        FORCEINLINE uint32 GetComponentCount() const { return components.GetSize(); }
        
		EXTENSIONS_API friend void Pup(Pupper* p, RyujinEntity& obj, const VarInfo& inVarInfo);
    };
}
