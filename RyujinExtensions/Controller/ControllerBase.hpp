#pragma once


#include "../Utils/ExtensionsBase.hpp"
#include "RyujinCore/Containers/HashTable.hpp"

#include "RyujinCore/CoreUtils/Delegate.hpp"

namespace Ryujin
{
    enum class ControllerAction : uint8
    {
        CA_Forward,
        CA_Back,
        CA_Right,
        CA_Left,
        CA_Up,
        CA_Down,
        CA_Jump,
        CA_Count
    };
    
    static const char* controllerActionNames[] =
    {
        "forward",
        "back",
        "right",
        "left",
        "up",
        "down",
        "jump",
    };
    
    
    class EXTENSIONS_API Controller
    {
    protected:
        HashTable<uint32, ControllerAction> keyToAction;
        
    public:
        VIRTUAL bool Initialize(int32 playerIndex) = 0;
        VIRTUAL void Update(float dt) = 0;
        
        VIRTUAL bool ShouldWarpMouse() const { return false; }
        
        Delegate<ControllerAction, float> OnActionPerformed;
        Delegate<float, float, float, float> OnRotationActionPerformed;
    };
}
