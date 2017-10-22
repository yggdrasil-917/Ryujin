#pragma once

#include "ControllerBase.hpp"

#include "RyujinCore/Input/Keyboard.hpp"
#include "RyujinCore/Input/Mouse.hpp"


namespace Ryujin
{
    class EXTENSIONS_API PlayerController : public Controller
    {
    public:
        bool Initialize(int32 playerIndex) OVERRIDE FINAL;
        void Update(float dt) OVERRIDE FINAL;
        
        bool ShouldWarpMouse() const OVERRIDE FINAL { return true; }
    };
}
