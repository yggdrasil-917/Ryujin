#include "Entity.hpp"


namespace Ryujin
{
    RyujinEntity::~RyujinEntity()
    {
        for (auto& it : components)
        {
            SAFE_DELETE(it);
        }
    }
    
    uint32 RyujinEntity::AddComponent(Component* component)
    {
        if (!component)
        {
            return (uint32)-1;
        }
        
        component->entity = this;
        components.Add(component);
        return components.GetSize() - 1;
    }
    
    void Pup(Pupper* p, RyujinEntity& obj, const VarInfo& inVarInfo)
    {
        int32 length = obj.components.GetSize();
        Pup(p, length, inVarInfo.name + ".length");
        
        if (p->ioMode == Pupper::IoMode::Read)
        {
            obj.components.Resize(length);
            for (int32 i = 0; i < length; ++i)
            {
                uint32 id = 0;
                Pup(p, id, inVarInfo.name + ".componentType");
                Component* component = ComponentFactory::Create(id);
                component->Serialize(p, String::Printf("%s.Component_%u", *inVarInfo.name, component->GetTypeId()));
                obj.components.Add(component);
            }
        }
        else
        {
            for (auto& component : obj.components)
            {
                uint32 id = component->GetTypeId();
                Pup(p, id, inVarInfo.name + ".componentType");
                component->Serialize(p, String::Printf("%s.Component_%u", *inVarInfo.name, component->GetTypeId()));
            }
        }
    }
}
