#include "Events.hpp"

#include "../Input/ScancodeLinux.hpp"
#include "../Input/Mouse.hpp"
#include "../Window/WindowLinux.hpp"

namespace Ryujin
{
    void HandleEvent(const xcb_generic_event_t *event)
    {
        switch (event->response_type & 0x7f)
        {
        case XCB_CLIENT_MESSAGE:
//            if ((*(xcb_client_message_event_t*)event).data.data32[0] == (*atom_wm_delete_window).atom)
//            {
//                //quit = true;
//            }
            break;

        case XCB_MOTION_NOTIFY:
        {
            xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)event;
        }
            break;

        case XCB_BUTTON_PRESS:
        {
            xcb_button_press_event_t *press = (xcb_button_press_event_t *)event;
            switch (press->detail)
            {
            case XCB_BUTTON_INDEX_1: // left mouse button
                break;

            case XCB_BUTTON_INDEX_2: // middle
                break;

            case XCB_BUTTON_INDEX_3: // right
                break;
            }
        }
            break;

        case XCB_BUTTON_RELEASE:
        {
            xcb_button_press_event_t *press = (xcb_button_press_event_t *)event;
            switch (press->detail)
            {
            case XCB_BUTTON_INDEX_1: // left mouse button
                break;

            case XCB_BUTTON_INDEX_2: // middle
                break;

            case XCB_BUTTON_INDEX_3: // right
                break;
            }
        }
            break;

        case XCB_KEY_PRESS:
        {
            const xcb_key_release_event_t *keyEvent = (const xcb_key_release_event_t *)event;
            switch (keyEvent->detail)
            {

            }
        }
            break;

        case XCB_KEY_RELEASE:
        {
            const xcb_key_release_event_t *keyEvent = (const xcb_key_release_event_t *)event;
            RyujinScanCode ryujinScancode = countof(scancodeTableLinux) < keyEvent->detail ? RyujinScanCode::RYUJIN_SCANCODE_UNKNOWN : scancodeTableLinux[keyEvent->detail];

        }
            break;

        case XCB_DESTROY_NOTIFY:
            // quit = true;
            break;

        case XCB_CONFIGURE_NOTIFY:
        {
            const xcb_configure_notify_event_t *cfgEvent = (const xcb_configure_notify_event_t *)event;
            // handle window resize
        }
            break;
        }
    }

    void PumpEvents_Linux()
    {
        RenderContext renderContext = WindowLinux::GetCurrentFocusedWindow()->GetRenderContext();
        xcb_flush(renderContext);
        xcb_generic_event_t *event = xcb_poll_for_event(renderContext);
        if (event)
        {
            HandleEvent(event);
            free(event);
        }
    }
}
