#include "Keyboard.hpp"

#include "../Allocators/Memory.hpp"


namespace Ryujin
{
    Keyboard* Keyboard::Instance()
    {
        static Keyboard keyboard;
        return &keyboard;
    }

    Keyboard::Keyboard()
    {
        ResetKeyboardState();

        Memory::MemCpy(keyboardMap, (void*)&defaultKeymap[0], sizeof(uint8) * RYUJIN_NUM_SCANCODES);
    }

    //Keyboard::~Keyboard()
    //{
    //
    //}


    void Keyboard::ResetKeyboardState()
    {
        Memory::MemSet(keyState, KEY_RELEASED, sizeof(uint8) * RYUJIN_NUM_SCANCODES);
        modState = KeyMod::None;
    }


    void Keyboard::SetKeyState(RyujinScanCode key, uint8 state)
    {
        keyState[key] = state;

        if (state == KEY_PRESSED)
        {
            switch (key)
            {
            case RYUJIN_SCANCODE_LSHIFT:
                modState |= KeyMod::LShift;
                break;

            case RYUJIN_SCANCODE_RSHIFT:
                modState |= KeyMod::RShift;
                break;

            case RYUJIN_SCANCODE_LCTRL:
                modState |= KeyMod::LCtrl;
                break;

            case RYUJIN_SCANCODE_RCTRL:
                modState |= KeyMod::RCtrl;
                break;

            case RYUJIN_SCANCODE_LALT:
                modState |= KeyMod::LAlt;
                break;

            case RYUJIN_SCANCODE_RALT:
                modState |= KeyMod::RAlt;
                break;
            }
        }
    }

}


