#include "Viewport.hpp"

#include "../Input/Events.hpp"

#if PLATFORM_WINDOWS
#include "WindowWindows.hpp"

#elif PLATFORM_MAC
#include "WindowMac.hpp"

#elif PLATFORM_LINUX
#include "WindowLinux.hpp"

#endif


namespace Ryujin
{

    Viewport::Viewport()
    {
        clientWidth = 640;
        clientHeight = 360;

        fullscreen = false;

    #if PLATFORM_WINDOWS
        window = RYUJIN_NEW WindowWindows();
    #elif PLATFORM_MAC
        window = RYUJIN_NEW WindowMac();
    #elif PLATFORM_LINUX
        window = RYUJIN_NEW WindowLinux();
    #else
        window = nullptr;
    #endif
    }

    Viewport::~Viewport()
    {
        delete window;
    }

    void Viewport::CheckFullscreenChange(bool inFullscreen)
    {
        if (inFullscreen && !IsFullscreen())
        {
            SetFullscreen(true);
        }
        else if (!inFullscreen && IsFullscreen())
        {
            SetFullscreen(false);
        }
    }

    void Viewport::WarpMouse(uint32 x, uint32 y)
    {
    #if PLATFORM_WINDOWS
        POINT p;
        p.x = x;
        p.y = y;
        ClientToScreen(window->GetWindowHandle(), &p);
        SetCursorPos(p.x, p.y);
        Mouse::Instance()->UpdateCursorLocation(x, y);
    #elif PLATFORM_MAC
        WarpMouseCocoa(window->GetWindowHandle(), x, y);
    #endif
    }

    void Viewport::SetMouseCursorVisible(bool bIsVisible)
    {
    #if PLATFORM_WINDOWS
        ShowCursor(bIsVisible);
    #elif PLATFORM_MAC
        ShowCursorCocoa(bIsVisible);
    #endif
    }

    void Viewport::Present()
    {
    #if PLATFORM_WINDOWS
        SwapBuffers(window->GetDeviceContext());
    #endif
    }

    void Viewport::Close()
    {
        
    }
    
    bool Viewport::CreateWindow(const WindowDescriptor& desc)
    {
        if (!window->BuildWindow(desc))
            return false;
        
        window->SetVSync(0);
        
        return true;
    }
    
}
