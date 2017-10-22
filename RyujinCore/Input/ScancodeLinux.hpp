
#pragma once


#include "Scancode.hpp"



namespace Ryujin
{
    static const RyujinScanCode scancodeTableLinux[] =
    {
        /*   0 */   RYUJIN_SCANCODE_A,
        /*   1 */   RYUJIN_SCANCODE_S,
        /*   2 */   RYUJIN_SCANCODE_D,
        /*   3 */   RYUJIN_SCANCODE_F,
        /*   4 */   RYUJIN_SCANCODE_H,
        /*   5 */   RYUJIN_SCANCODE_G,
        /*   6 */   RYUJIN_SCANCODE_Z,
        /*   7 */   RYUJIN_SCANCODE_X,
        /*   8 */   RYUJIN_SCANCODE_C,
        /*   9 */   RYUJIN_SCANCODE_V,
        /*  10 */   RYUJIN_SCANCODE_NONUSBACKSLASH, /* RYUJIN_SCANCODE_NONUSBACKSLASH on ANSI and JIS keyboards (if this key would exist there), RYUJIN_SCANCODE_GRAVE on ISO. (The USB keyboard driver actually translates these usage codes to different virtual key codes depending on whether the keyboard is ISO/ANSI/JIS. That's why you have to help it identify the keyboard type when you plug in a PC USB keyboard. It's a historical thing - ADB keyboards are wired this way.) */
        /*  11 */   RYUJIN_SCANCODE_B,
        /*  12 */   RYUJIN_SCANCODE_Q,
        /*  13 */   RYUJIN_SCANCODE_W,
        /*  14 */   RYUJIN_SCANCODE_E,
        /*  15 */   RYUJIN_SCANCODE_R,
        /*  16 */   RYUJIN_SCANCODE_Y,
        /*  17 */   RYUJIN_SCANCODE_T,
        /*  18 */   RYUJIN_SCANCODE_1,
        /*  19 */   RYUJIN_SCANCODE_2,
        /*  20 */   RYUJIN_SCANCODE_3,
        /*  21 */   RYUJIN_SCANCODE_4,
        /*  22 */   RYUJIN_SCANCODE_6,
        /*  23 */   RYUJIN_SCANCODE_5,
        /*  24 */   RYUJIN_SCANCODE_EQUALS,
        /*  25 */   RYUJIN_SCANCODE_9,
        /*  26 */   RYUJIN_SCANCODE_7,
        /*  27 */   RYUJIN_SCANCODE_MINUS,
        /*  28 */   RYUJIN_SCANCODE_8,
        /*  29 */   RYUJIN_SCANCODE_0,
        /*  30 */   RYUJIN_SCANCODE_RIGHTBRACKET,
        /*  31 */   RYUJIN_SCANCODE_O,
        /*  32 */   RYUJIN_SCANCODE_U,
        /*  33 */   RYUJIN_SCANCODE_LEFTBRACKET,
        /*  34 */   RYUJIN_SCANCODE_I,
        /*  35 */   RYUJIN_SCANCODE_P,
        /*  36 */   RYUJIN_SCANCODE_RETURN,
        /*  37 */   RYUJIN_SCANCODE_L,
        /*  38 */   RYUJIN_SCANCODE_J,
        /*  39 */   RYUJIN_SCANCODE_APOSTROPHE,
        /*  40 */   RYUJIN_SCANCODE_K,
        /*  41 */   RYUJIN_SCANCODE_SEMICOLON,
        /*  42 */   RYUJIN_SCANCODE_BACKSLASH,
        /*  43 */   RYUJIN_SCANCODE_COMMA,
        /*  44 */   RYUJIN_SCANCODE_SLASH,
        /*  45 */   RYUJIN_SCANCODE_N,
        /*  46 */   RYUJIN_SCANCODE_M,
        /*  47 */   RYUJIN_SCANCODE_PERIOD,
        /*  48 */   RYUJIN_SCANCODE_TAB,
        /*  49 */   RYUJIN_SCANCODE_SPACE,
        /*  50 */   RYUJIN_SCANCODE_GRAVE, /* RYUJIN_SCANCODE_GRAVE on ANSI and JIS keyboards, RYUJIN_SCANCODE_NONUSBACKSLASH on ISO (see comment about virtual key code 10 above) */
        /*  51 */   RYUJIN_SCANCODE_BACKSPACE,
        /*  52 */   RYUJIN_SCANCODE_KP_ENTER, /* keyboard enter on portables */
        /*  53 */   RYUJIN_SCANCODE_ESCAPE,
        /*  54 */   RYUJIN_SCANCODE_RGUI,
        /*  55 */   RYUJIN_SCANCODE_LGUI,
        /*  56 */   RYUJIN_SCANCODE_LSHIFT,
        /*  57 */   RYUJIN_SCANCODE_CAPSLOCK,
        /*  58 */   RYUJIN_SCANCODE_LALT,
        /*  59 */   RYUJIN_SCANCODE_LCTRL,
        /*  60 */   RYUJIN_SCANCODE_RSHIFT,
        /*  61 */   RYUJIN_SCANCODE_RALT,
        /*  62 */   RYUJIN_SCANCODE_RCTRL,
        /*  63 */   RYUJIN_SCANCODE_RGUI, /* fn on portables, acts as a hardware-level modifier already, so we don't generate events for it, also XK_Meta_R */
        /*  64 */   RYUJIN_SCANCODE_F17,
        /*  65 */   RYUJIN_SCANCODE_KP_PERIOD,
        /*  66 */   RYUJIN_SCANCODE_UNKNOWN, /* unknown (unused?) */
        /*  67 */   RYUJIN_SCANCODE_KP_MULTIPLY,
        /*  68 */   RYUJIN_SCANCODE_UNKNOWN, /* unknown (unused?) */
        /*  69 */   RYUJIN_SCANCODE_KP_PLUS,
        /*  70 */   RYUJIN_SCANCODE_UNKNOWN, /* unknown (unused?) */
        /*  71 */   RYUJIN_SCANCODE_NUMLOCKCLEAR,
        /*  72 */   RYUJIN_SCANCODE_VOLUMEUP,
        /*  73 */   RYUJIN_SCANCODE_VOLUMEDOWN,
        /*  74 */   RYUJIN_SCANCODE_MUTE,
        /*  75 */   RYUJIN_SCANCODE_KP_DIVIDE,
        /*  76 */   RYUJIN_SCANCODE_KP_ENTER, /* keypad enter on external keyboards, fn-return on portables */
        /*  77 */   RYUJIN_SCANCODE_UNKNOWN, /* unknown (unused?) */
        /*  78 */   RYUJIN_SCANCODE_KP_MINUS,
        /*  79 */   RYUJIN_SCANCODE_F18,
        /*  80 */   RYUJIN_SCANCODE_F19,
        /*  81 */   RYUJIN_SCANCODE_KP_EQUALS,
        /*  82 */   RYUJIN_SCANCODE_KP_0,
        /*  83 */   RYUJIN_SCANCODE_KP_1,
        /*  84 */   RYUJIN_SCANCODE_KP_2,
        /*  85 */   RYUJIN_SCANCODE_KP_3,
        /*  86 */   RYUJIN_SCANCODE_KP_4,
        /*  87 */   RYUJIN_SCANCODE_KP_5,
        /*  88 */   RYUJIN_SCANCODE_KP_6,
        /*  89 */   RYUJIN_SCANCODE_KP_7,
        /*  90 */   RYUJIN_SCANCODE_UNKNOWN, /* unknown (unused?) */
        /*  91 */   RYUJIN_SCANCODE_KP_8,
        /*  92 */   RYUJIN_SCANCODE_KP_9,
        /*  93 */   RYUJIN_SCANCODE_INTERNATIONAL3, /* Cosmo_USB2ADB.c says "Yen (JIS)" */
        /*  94 */   RYUJIN_SCANCODE_INTERNATIONAL1, /* Cosmo_USB2ADB.c says "Ro (JIS)" */
        /*  95 */   RYUJIN_SCANCODE_KP_COMMA, /* Cosmo_USB2ADB.c says ", JIS only" */
        /*  96 */   RYUJIN_SCANCODE_F5,
        /*  97 */   RYUJIN_SCANCODE_F6,
        /*  98 */   RYUJIN_SCANCODE_F7,
        /*  99 */   RYUJIN_SCANCODE_F3,
        /* 100 */   RYUJIN_SCANCODE_F8,
        /* 101 */   RYUJIN_SCANCODE_F9,
        /* 102 */   RYUJIN_SCANCODE_LANG2, /* Cosmo_USB2ADB.c says "Eisu" */
        /* 103 */   RYUJIN_SCANCODE_F11,
        /* 104 */   RYUJIN_SCANCODE_LANG1, /* Cosmo_USB2ADB.c says "Kana" */
        /* 105 */   RYUJIN_SCANCODE_PRINTSCREEN, /* On ADB keyboards, this key is labeled "F13/print screen". Problem: USB has different usage codes for these two functions. On Apple USB keyboards, the key is labeled "F13" and sends the F13 usage code (RYUJIN_SCANCODE_F13). I decided to use RYUJIN_SCANCODE_PRINTSCREEN here nevertheless since SDL applications are more likely to assume the presence of a print screen key than an F13 key. */
        /* 106 */   RYUJIN_SCANCODE_F16,
        /* 107 */   RYUJIN_SCANCODE_SCROLLLOCK, /* F14/scroll lock, see comment about F13/print screen above */
        /* 108 */   RYUJIN_SCANCODE_UNKNOWN, /* unknown (unused?) */
        /* 109 */   RYUJIN_SCANCODE_F10,
        /* 110 */   RYUJIN_SCANCODE_APPLICATION, /* windows contextual menu key, fn-enter on portables */
        /* 111 */   RYUJIN_SCANCODE_F12,
        /* 112 */   RYUJIN_SCANCODE_UNKNOWN, /* unknown (unused?) */
        /* 113 */   RYUJIN_SCANCODE_PAUSE, /* F15/pause, see comment about F13/print screen above */
        /* 114 */   RYUJIN_SCANCODE_INSERT, /* the key is actually labeled "help" on Apple keyboards, and works as such in Mac OS, but it sends the "insert" usage code even on Apple USB keyboards */
        /* 115 */   RYUJIN_SCANCODE_HOME,
        /* 116 */   RYUJIN_SCANCODE_PAGEUP,
        /* 117 */   RYUJIN_SCANCODE_DELETE,
        /* 118 */   RYUJIN_SCANCODE_F4,
        /* 119 */   RYUJIN_SCANCODE_END,
        /* 120 */   RYUJIN_SCANCODE_F2,
        /* 121 */   RYUJIN_SCANCODE_PAGEDOWN,
        /* 122 */   RYUJIN_SCANCODE_F1,
        /* 123 */   RYUJIN_SCANCODE_LEFT,
        /* 124 */   RYUJIN_SCANCODE_RIGHT,
        /* 125 */   RYUJIN_SCANCODE_DOWN,
        /* 126 */   RYUJIN_SCANCODE_UP,
        /* 127 */   RYUJIN_SCANCODE_POWER
    };
}
