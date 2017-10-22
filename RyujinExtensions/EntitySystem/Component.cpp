
#include "Component.hpp"


namespace Ryujin
{
    Component::Component()
    : TPuppableObject<Component>(*this),
      bIsEnabled(true)
    {
        
    }
    
    Component::Component(const Component& other)
    : TPuppableObject<Component>(*this),
      bIsEnabled(other.bIsEnabled)
    {
        
    }
    
    Component::Component(Component&& other)
    : TPuppableObject<Component>(*this),
      bIsEnabled(other.bIsEnabled)
    {
        
    }
    
    Component& Component::operator=(const Component& other)
    {
        bIsEnabled = other.bIsEnabled;
        return *this;
    }
    
    Component& Component::operator=(Component&& other)
    {
        bIsEnabled = other.bIsEnabled;
        return *this;
    }
    
    void Component::Serialize(Pupper* p, const VarInfo& inVarInfo)
    {
        Pup(p, *this, inVarInfo);
    }
    
    void Pup(Pupper* p, Component& obj, const VarInfo& inVarInfo)
    {
        Pup(p, obj.bIsEnabled, inVarInfo.name + ".enabled");
    }
    
    IMPLEMENT_COMPONENT_TYPE(TagComponent);
    
    TagComponent::TagComponent(const char* tag)
    : tag(tag)
    {
        
    }
    
    TagComponent::TagComponent(const TagComponent& other)
    : Component(),
      tag(other.tag)
    {
        
    }
    
    TagComponent::TagComponent(TagComponent&& other)
    : Component(),
      tag(Move(other.tag))
    {
        
    }
    
    TagComponent& TagComponent::operator=(const TagComponent& other)
    {
        bIsEnabled = other.bIsEnabled;
        tag = other.tag;
        return *this;
    }
    
    TagComponent& TagComponent::operator=(TagComponent&& other)
    {
        bIsEnabled = other.bIsEnabled;
        tag = Move(other.tag);
        return *this;
    }
    
    void TagComponent::Serialize(Pupper* p, const VarInfo& inVarInfo)
    {
        Component::Serialize(p, inVarInfo);
        
        Pup(p, *this, inVarInfo);
    }
    
    void Pup(Pupper* p, TagComponent& obj, const VarInfo& inVarInfo)
    {
        Pup(p, obj.tag, inVarInfo.name + ".tag");
    }
    
    ComponentCreator::ComponentCreator(const char* className)
    {
        ComponentFactory::Register((uint32)CalculateFNV(className), this);
    }
    
    Component* ComponentFactory::Create(const uint32 classId)
    {
        ComponentCreator* creator = nullptr;
        if (GetTable().TryGetValue(classId, creator))
        {
            return creator->Create();
        }
        
        return nullptr;
    }
    
    void ComponentFactory::Register(const uint32 classId, ComponentCreator *creator)
    {
        GetTable()[classId] = creator;
    }
    
    HashTable<uint32, ComponentCreator*>& ComponentFactory::GetTable()
    {
        static HashTable<uint32, ComponentCreator*> table(1024);
        return table;
    }
}
