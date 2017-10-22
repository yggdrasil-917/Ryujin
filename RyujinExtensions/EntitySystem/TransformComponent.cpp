
#include "TransformComponent.hpp"


namespace Ryujin
{
    IMPLEMENT_COMPONENT_TYPE(TransformComponent);
    
    TransformComponent::TransformComponent()
    : Component()
    {
        
    }
    
    TransformComponent::TransformComponent(const Transform& other)
    : Component(),
      transform(other)
    {
        
    }

	TransformComponent::TransformComponent(const TransformComponent& other)
	: Component(other),
	  transform(other.transform)
	{

	}
    
    TransformComponent::TransformComponent(TransformComponent&& other)
    : Component(other),
      transform(Move(other.transform))
    {
        
    }
    
    TransformComponent& TransformComponent::operator=(const TransformComponent& other)
    {
        bIsEnabled = other.bIsEnabled;
        transform = other.transform;
        return *this;
    }
    
    TransformComponent& TransformComponent::operator=(TransformComponent&& other)
    {
        bIsEnabled = other.bIsEnabled;
        transform = Move(other.transform);
        return *this;
    }
    
    void TransformComponent::Serialize(Pupper* p, const VarInfo& inVarInfo)
    {
        Component::Serialize(p, inVarInfo);
        
        Pup(p, *this, inVarInfo);
    }
    
    void Pup(Pupper* p, TransformComponent& obj, const VarInfo& inVarInfo)
    {
        Pup(p, obj.transform, inVarInfo.name + ".transform");
    }
}
