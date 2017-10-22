#pragma once

#include "RyujinCore/Math/Transform.hpp"
#include "Component.hpp"


namespace Ryujin
{
    class EXTENSIONS_API TransformComponent : public Component
    {
        DECLARE_COMPONENT_TYPE(TransformComponent);
    private:
        Transform transform;
        
    public:
        TransformComponent();
        TransformComponent(const Transform& other);
        
        TransformComponent(const TransformComponent& other);
        TransformComponent(TransformComponent&& other);
        
        TransformComponent& operator=(const TransformComponent& other);
        TransformComponent& operator=(TransformComponent&& other);
        
        void Serialize(Pupper* p, const VarInfo& inVarInfo) OVERRIDE FINAL;
        
        FORCEINLINE Transform& GetTransform() { return transform; }
        
		EXTENSIONS_API friend void Pup(Pupper* p, TransformComponent& obj, const VarInfo& inVarInfo);
    };
}
