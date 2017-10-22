
#pragma once


#include "Window.hpp"


namespace Ryujin
{
    class CORE_API WindowLinux : public Window
    {
    private:
        static WindowLinux* currentFocusedWindow;
        xcb_intern_atom_reply_t* atomWmDeleteWindow;

        bool BuildNormalWindow(const WindowDescriptor& winDesc) OVERRIDE FINAL;
        bool BuildSplashScreen(const WindowDescriptor& winDesc) OVERRIDE FINAL;

        bool InitXcbConnection();

    public:
        ~WindowLinux();

        bool BuildWindow(const WindowDescriptor& winDesc) OVERRIDE FINAL;
        void DestroyWindow() OVERRIDE FINAL;
        void Resize(const uint32 inWidth, const uint32 inHeight) OVERRIDE FINAL;
        void ToggleFullscreen(bool bEnableFullscreen) OVERRIDE FINAL;
        void SetWindowVisibility(bool bShow) OVERRIDE FINAL;

        void SetVSync(const int32 vsync) OVERRIDE FINAL;

        static WindowLinux* GetCurrentFocusedWindow() { return currentFocusedWindow; }
    };
}
