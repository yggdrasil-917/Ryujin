#include "ImGuiManager.hpp"

#include "RyujinRenderer/Renderer/Renderer.hpp"
#include "RyujinCore/Input/Keycode.hpp"

namespace Ryujin
{
    void ImGuiManager::Init()
    {
        ImGuiIO& io = ImGui::GetIO();
        
        io.KeyMap[ImGuiKey_Tab] = RYUJIN_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = RYUJIN_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = RYUJIN_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = RYUJIN_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = RYUJIN_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = RYUJIN_KEY_PAGEUP;
        io.KeyMap[ImGuiKey_PageDown] = RYUJIN_KEY_PAGEDOWN;
        io.KeyMap[ImGuiKey_Home] = RYUJIN_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = RYUJIN_KEY_END;
        io.KeyMap[ImGuiKey_Delete] = RYUJIN_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = RYUJIN_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Enter] = RYUJIN_KEY_RETURN;
        io.KeyMap[ImGuiKey_Escape] = RYUJIN_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A] = RYUJIN_KEY_a;
        io.KeyMap[ImGuiKey_C] = RYUJIN_KEY_c;
        io.KeyMap[ImGuiKey_V] = RYUJIN_KEY_v;
        io.KeyMap[ImGuiKey_X] = RYUJIN_KEY_x;
        io.KeyMap[ImGuiKey_Y] = RYUJIN_KEY_y;
        io.KeyMap[ImGuiKey_Z] = RYUJIN_KEY_z;
    }
    
    void ImGuiManager::Shutdown()
    {
        
    }
    
    void ImGuiManager::BeginFrame()
    {
        
    }
    
    void ImGuiManager::EndFrame()
    {
        
    }
    
    void ImGuiManager::Resize(uint16 windowWidth, uint16 windowHeight, uint16 frameBufferWidth, uint16 frameBufferHeight)
    {
        
    }
}
