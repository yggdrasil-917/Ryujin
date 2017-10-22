#include "WindowLinux.hpp"


namespace Ryujin
{
    WindowLinux* WindowLinux::currentFocusedWindow = nullptr;

    bool WindowLinux::InitXcbConnection()
    {
        const xcb_setup_t* setup;
        xcb_screen_iterator_t iter;
        int32 scr;

        renderContext = xcb_connect(nullptr, &scr);
        if (renderContext == nullptr)
        {
            return false;
        }

        setup = xcb_get_setup(renderContext);
        iter = xcb_setup_roots_iterator(setup);
        while (scr-- > 0)
            xcb_screen_next(&iter);
        deviceContext = iter.data;

        return true;
    }

    bool WindowLinux::BuildNormalWindow(const WindowDescriptor &winDesc)
    {
        if (!InitXcbConnection())
        {
            return false;
        }

        windowHandle = xcb_generate_id(renderContext);
        uint32 valueMask, valueList[32];

        valueMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
        valueList[0] = deviceContext->black_pixel;
        valueList[1] =
            XCB_EVENT_MASK_KEY_RELEASE |
            XCB_EVENT_MASK_KEY_PRESS |
            XCB_EVENT_MASK_EXPOSURE |
            XCB_EVENT_MASK_STRUCTURE_NOTIFY |
            XCB_EVENT_MASK_POINTER_MOTION |
            XCB_EVENT_MASK_BUTTON_PRESS |
            XCB_EVENT_MASK_BUTTON_RELEASE;

        xcb_create_window(renderContext,
            XCB_COPY_FROM_PARENT,
            windowHandle, deviceContext->root,
            0, 0, winDesc.width, winDesc.height, 0,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            deviceContext->root_visual,
            valueMask, valueList);

        /* Magic code that will send notification when window is destroyed */
        xcb_intern_atom_cookie_t cookie = xcb_intern_atom(renderContext, 1, 12, "WM_PROTOCOLS");
        xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(renderContext, cookie, 0);

        xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(renderContext, 0, 16, "WM_DELETE_WINDOW");
        atomWmDeleteWindow = xcb_intern_atom_reply(renderContext, cookie2, 0);

        xcb_change_property(renderContext, XCB_PROP_MODE_REPLACE,
            windowHandle, (*reply).atom, 4, 32, 1,
            &(*atomWmDeleteWindow).atom);

        xcb_change_property(renderContext, XCB_PROP_MODE_REPLACE,
            windowHandle, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
            winDesc.windowTitleOrSplashScreenPath.Length(), *winDesc.windowTitleOrSplashScreenPath);

        free(reply);

        xcb_map_window(renderContext, windowHandle);
        xcb_flush(renderContext);

        currentFocusedWindow = this;

        return true;
    }

    bool WindowLinux::BuildSplashScreen(const WindowDescriptor &winDesc)
    {
        return true;
    }

    WindowLinux::~WindowLinux()
    {
        DestroyWindow();
    }

    bool WindowLinux::BuildWindow(const WindowDescriptor &winDesc)
    {
        if (winDesc.bIsSplashScreen)
        {
            return BuildSplashScreen(winDesc);
        }
        else
        {
            return BuildNormalWindow(winDesc);
        }
    }

    void WindowLinux::DestroyWindow()
    {
        if (renderContext && windowHandle)
        {
            xcb_destroy_window(renderContext, windowHandle);
            xcb_disconnect(renderContext);
        }
    }

    void WindowLinux::Resize(const uint32 inWidth, const uint32 inHeight)
    {

    }

    void WindowLinux::ToggleFullscreen(bool bEnableFullscreen)
    {

    }

    void WindowLinux::SetWindowVisibility(bool bShow)
    {

    }

    void WindowLinux::SetVSync(const int32 vsync)
    {

    }
}
