#pragma once


#include "RyujinCore/OS/Types.hpp"

#include "imgui/imgui.h"

namespace Ryujin
{
    class ImGuiManager
    {
    public:
        void Init();
        void Shutdown();
        
        void BeginFrame();
        void EndFrame();
        void Resize(uint16 windowWidth, uint16 windowHeight, uint16 frameBufferWidth, uint16 frameBufferHeight);
    };
}
