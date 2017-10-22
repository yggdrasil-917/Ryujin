
#include "../Input/Events.hpp"
#include "../Input/ScancodeDarwin.hpp"
#include "../Input/Mouse.hpp"
#include "../Window/WindowMac.hpp"


namespace Ryujin
{
    static bool bIsCursorVisible = true;
    void ShowCursorCocoa(bool bIsVisible)
    { @autoreleasepool
    {
        if (bIsVisible && !bIsCursorVisible)
        {
            [NSCursor unhide];
            bIsCursorVisible = true;
//            NSLog(@"Cursor unhide called");
        }
        else if (!bIsVisible && bIsCursorVisible)
        {
//            NSLog(@"Cursor hide called");
            [NSCursor hide];
            bIsCursorVisible = false;
        }
    }
    }
    
    void WarpMouseCocoa(void* windowHandle, uint32 x, uint32 y)
    { @autoreleasepool
    {
        // Screen origin is top left
        // Window origin is bottom left
        RyujinWindow* window = (RyujinWindow*)windowHandle;
        NSRect frame = [window frame];
        NSRect primaryMonitor = [[NSScreen mainScreen] frame];
        NSPoint pointInScreenSpace;
        
        pointInScreenSpace.x = frame.origin.x + x;
        pointInScreenSpace.y = frame.origin.y + y;
        pointInScreenSpace.y = primaryMonitor.size.height - pointInScreenSpace.y;
        CGWarpMouseCursorPosition(pointInScreenSpace);
        CGAssociateMouseAndMouseCursorPosition(true);
        Mouse::Instance()->WarpMouse(x, y);
//        Mouse::Instance()->UpdateCursorLocation((int32)pointInScreenSpace.x, (int32)pointInScreenSpace.y);
        //NSLog(@"%f %f", pointInScreenSpace.x, pointInScreenSpace.y);
    }
    }
    
    void HandleMouseEventCocoa(NSEvent* inEvent)
    { @autoreleasepool
    {
        const NSUInteger cocoaButtons = [NSEvent pressedMouseButtons];
        
        uint32 buttonsPressed = 0;
        buttonsPressed |= (cocoaButtons & (1 << 0)) ? MOUSE_BUTTON(MouseButton::LButton) : 0;
        buttonsPressed |= (cocoaButtons & (1 << 1)) ? MOUSE_BUTTON(MouseButton::RButton) : 0;
        buttonsPressed |= (cocoaButtons & (1 << 2)) ? MOUSE_BUTTON(MouseButton::MButton) : 0;
        buttonsPressed |= (cocoaButtons & (1 << 3)) ? MOUSE_BUTTON(MouseButton::WheelUp) : 0;
        buttonsPressed |= (cocoaButtons & (1 << 4)) ? MOUSE_BUTTON(MouseButton::WheelDown) : 0;
        
        Mouse* mouse = Mouse::Instance();
        if (buttonsPressed == mouse->GetButtonFlags())
            return;
        
        mouse->UpdateMouseButtonState(MouseButton::LButton, (cocoaButtons & (1 << 0)) ? RyujinState::Pressed : RyujinState::Released);
        mouse->UpdateMouseButtonState(MouseButton::MButton, (cocoaButtons & (1 << 2)) ? RyujinState::Pressed : RyujinState::Released);
        mouse->UpdateMouseButtonState(MouseButton::RButton, (cocoaButtons & (1 << 1)) ? RyujinState::Pressed : RyujinState::Released);
        
        mouse->CacheMouseButtonState(buttonsPressed);
    }
    }
    
    void HandleModifiersCocoa(NSEvent* inEvent)
    { @autoreleasepool
    {
        const NSEventModifierFlags cocoaModFlags = [inEvent modifierFlags];
        Keyboard* keyboard = Keyboard::Instance();
        keyboard->SetKeyState(RYUJIN_SCANCODE_LSHIFT, cocoaModFlags & NSEventModifierFlagShift ? KEY_PRESSED : KEY_RELEASED);
        keyboard->SetKeyState(RYUJIN_SCANCODE_LCTRL, cocoaModFlags & NSEventModifierFlagControl ? KEY_PRESSED : KEY_RELEASED);
        keyboard->SetKeyState(RYUJIN_SCANCODE_LALT, cocoaModFlags & NSEventModifierFlagOption ? KEY_PRESSED : KEY_RELEASED);
        keyboard->SetKeyState(RYUJIN_SCANCODE_LGUI, cocoaModFlags & NSEventModifierFlagCommand ? KEY_PRESSED : KEY_RELEASED);
    }
    }
    
    void HandleKeyboardEventCocoa(NSEvent* inEvent)
    { @autoreleasepool
    {
        uint16 scancode = [inEvent keyCode];
        RyujinScanCode RyujinScancode;
        if (countof(scancodeTableDarwin) < scancode)
        {
            RyujinScancode = RyujinScanCode::RYUJIN_SCANCODE_UNKNOWN;
        }
        else
        {
            RyujinScancode = scancodeTableDarwin[scancode];
        }
        
        switch ([inEvent type])
        {
            case NSEventTypeKeyDown:
                if (![inEvent isARepeat])
                {
                    Keyboard::Instance()->SetKeyState(RyujinScancode, KEY_PRESSED);
                }
                PushKeyboardEvent(EngineEvent::KeyDown, Keyboard::Instance()->GetKeyMap(RyujinScancode), Keyboard::Instance()->GetModState());
                break;
                
            case NSEventTypeKeyUp:
            {
                Keyboard::Instance()->SetKeyState(RyujinScancode, KEY_RELEASED);
                PushKeyboardEvent(EngineEvent::KeyUp, Keyboard::Instance()->GetKeyMap(RyujinScancode), Keyboard::Instance()->GetModState());
                
                const uint8* keyState = Keyboard::Instance()->GetKeyboardState();
                if (keyState[RYUJIN_SCANCODE_LGUI] || keyState[RYUJIN_SCANCODE_RGUI])
                {
                    if (RyujinScancode == RYUJIN_SCANCODE_Q)
                        PushWindowEvent(WindowEventID::Close);
                }
            }
                break;
                
            case NSEventTypeFlagsChanged:
                HandleModifiersCocoa(inEvent);
                break;
                
            default:
                break;
        }
    }
    }
    
    BOOL ConvertNSEventToRyujinEvent(NSEvent* inEvent)
    { @autoreleasepool
    {
        if (!inEvent)
        {
            return NO;
        }
        
        switch ([inEvent type])
        {
            case NSEventTypeLeftMouseDown:
            case NSEventTypeOtherMouseDown:
            case NSEventTypeRightMouseDown:
            case NSEventTypeLeftMouseUp:
            case NSEventTypeOtherMouseUp:
            case NSEventTypeRightMouseUp:
            case NSEventTypeLeftMouseDragged:
            case NSEventTypeRightMouseDragged:
            case NSEventTypeOtherMouseDragged:
            //case NSMouseMoved: // Mouse move handled in RyujinWindow
            case NSEventTypeScrollWheel:
                HandleMouseEventCocoa(inEvent);
                return YES;
            case NSEventTypeKeyDown:
            case NSEventTypeKeyUp:
            case NSEventTypeFlagsChanged:
                HandleKeyboardEventCocoa(inEvent);
                return YES;
                
//            default:
//                [NSApp sendEvent:inEvent];
//                return NO;
        }
        
        return NO;
    }
    }
    
    void PumpEvents_Mac()
    { @autoreleasepool
    {
        while (true)
        {
            NSEvent *event =
            [NSApp
             nextEventMatchingMask:NSEventMaskAny
             untilDate:[NSDate distantPast]
             inMode:NSDefaultRunLoopMode
             dequeue:YES];
            
            if (!event)
                break;
            
            ConvertNSEventToRyujinEvent(event);
            [NSApp sendEvent:event];
        }
    }
    }
}
