#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "Keycode.hpp"
#include "../Logger/Logger.hpp"
#include "../CoreUtils/Singleton.hpp"

#include <type_traits> // for std::underlying_type

namespace Ryujin
{
#define KEY_PRESSED 1
#define KEY_RELEASED 0

	enum class KeyMod : uint16
	{
		None = 0x0000,
		LShift = 0x0001,
		RShift = 0x0002,
		LCtrl = 0x0004,
		RCtrl = 0x0008,
		LAlt = 0x0010,
		RAlt = 0x0020,
	};

	using T = std::underlying_type<KeyMod>::type;

	// define bitwise operator overloads for KeyMod enum
	FORCEINLINE KeyMod operator |(KeyMod lhs, KeyMod rhs)
	{
		return (KeyMod)(static_cast<T>(lhs) | static_cast<T>(rhs));
	}

	FORCEINLINE KeyMod& operator |=(KeyMod& lhs, KeyMod rhs)
	{
		lhs = (KeyMod)(static_cast<T>(lhs) | static_cast<T>(rhs));
		return lhs;
	}

#define KEYMOD_CTRL (KeyMod::LCtrl | KeyMod::RCtrl)
#define KEYMOD_ALT (KeyMod::LAlt | KeyMod::RAlt)
#define KEYMOD_SHIFT (KeyMod::LShift | KeyMod::RShift)

	class CORE_API Keyboard// : public Singleton<Keyboard>
	{
	private:
		uint8 keyState[RYUJIN_NUM_SCANCODES];
		KeyMod modState;
		int32 keyboardMap[RYUJIN_NUM_SCANCODES];

	public:
		Keyboard();
//		~Keyboard();
        
        static Keyboard* Instance();

		void ResetKeyboardState();
		void SetKeyState(RyujinScanCode key, uint8 state);

		FORCEINLINE int32 GetKeyMap(RyujinScanCode scanCode) const { return keyboardMap[scanCode]; }
		FORCEINLINE const uint8* GetKeyboardState() const { return keyState; }
		FORCEINLINE KeyMod GetModState() const { return modState; }
	};

	static const int32 defaultKeymap[RYUJIN_NUM_SCANCODES] =
	{
		0, 0, 0, 0,
		'a',
		'b',
		'c',
		'd',
		'e',
		'f',
		'g',
		'h',
		'i',
		'j',
		'k',
		'l',
		'm',
		'n',
		'o',
		'p',
		'q',
		'r',
		's',
		't',
		'u',
		'v',
		'w',
		'x',
		'y',
		'z',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'0',
		RYUJIN_KEY_RETURN,
		RYUJIN_KEY_ESCAPE,
		RYUJIN_KEY_BACKSPACE,
		RYUJIN_KEY_TAB,
		RYUJIN_KEY_SPACE,
		'-',
		'=',
		'[',
		']',
		'\\',
		'#',
		';',
		'\'',
		'`',
		',',
		'.',
		'/',
		RYUJIN_KEY_CAPSLOCK,
		RYUJIN_KEY_F1,
		RYUJIN_KEY_F2,
		RYUJIN_KEY_F3,
		RYUJIN_KEY_F4,
		RYUJIN_KEY_F5,
		RYUJIN_KEY_F6,
		RYUJIN_KEY_F7,
		RYUJIN_KEY_F8,
		RYUJIN_KEY_F9,
		RYUJIN_KEY_F10,
		RYUJIN_KEY_F11,
		RYUJIN_KEY_F12,
		RYUJIN_KEY_PRINTSCREEN,
		RYUJIN_KEY_SCROLLLOCK,
		RYUJIN_KEY_PAUSE,
		RYUJIN_KEY_INSERT,
		RYUJIN_KEY_HOME,
		RYUJIN_KEY_PAGEUP,
		RYUJIN_KEY_DELETE,
		RYUJIN_KEY_END,
		RYUJIN_KEY_PAGEDOWN,
		RYUJIN_KEY_RIGHT,
		RYUJIN_KEY_LEFT,
		RYUJIN_KEY_DOWN,
		RYUJIN_KEY_UP,
		RYUJIN_KEY_NUMLOCKCLEAR,
		RYUJIN_KEY_KP_DIVIDE,
		RYUJIN_KEY_KP_MULTIPLY,
		RYUJIN_KEY_KP_MINUS,
		RYUJIN_KEY_KP_PLUS,
		RYUJIN_KEY_KP_ENTER,
		RYUJIN_KEY_KP_1,
		RYUJIN_KEY_KP_2,
		RYUJIN_KEY_KP_3,
		RYUJIN_KEY_KP_4,
		RYUJIN_KEY_KP_5,
		RYUJIN_KEY_KP_6,
		RYUJIN_KEY_KP_7,
		RYUJIN_KEY_KP_8,
		RYUJIN_KEY_KP_9,
		RYUJIN_KEY_KP_0,
		RYUJIN_KEY_KP_PERIOD,
		0,
		RYUJIN_KEY_APPLICATION,
		RYUJIN_KEY_POWER,
		RYUJIN_KEY_KP_EQUALS,
		RYUJIN_KEY_F13,
		RYUJIN_KEY_F14,
		RYUJIN_KEY_F15,
		RYUJIN_KEY_F16,
		RYUJIN_KEY_F17,
		RYUJIN_KEY_F18,
		RYUJIN_KEY_F19,
		RYUJIN_KEY_F20,
		RYUJIN_KEY_F21,
		RYUJIN_KEY_F22,
		RYUJIN_KEY_F23,
		RYUJIN_KEY_F24,
		RYUJIN_KEY_EXECUTE,
		RYUJIN_KEY_HELP,
		RYUJIN_KEY_MENU,
		RYUJIN_KEY_SELECT,
		RYUJIN_KEY_STOP,
		RYUJIN_KEY_AGAIN,
		RYUJIN_KEY_UNDO,
		RYUJIN_KEY_CUT,
		RYUJIN_KEY_COPY,
		RYUJIN_KEY_PASTE,
		RYUJIN_KEY_FIND,
		RYUJIN_KEY_MUTE,
		RYUJIN_KEY_VOLUMEUP,
		RYUJIN_KEY_VOLUMEDOWN,
		0, 0, 0,
		RYUJIN_KEY_KP_COMMA,
		RYUJIN_KEY_KP_EQUALSAS400,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		RYUJIN_KEY_ALTERASE,
		RYUJIN_KEY_SYSREQ,
		RYUJIN_KEY_CANCEL,
		RYUJIN_KEY_CLEAR,
		RYUJIN_KEY_PRIOR,
		RYUJIN_KEY_RETURN2,
		RYUJIN_KEY_SEPARATOR,
		RYUJIN_KEY_OUT,
		RYUJIN_KEY_OPER,
		RYUJIN_KEY_CLEARAGAIN,
		RYUJIN_KEY_CRSEL,
		RYUJIN_KEY_EXSEL,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		RYUJIN_KEY_KP_00,
		RYUJIN_KEY_KP_000,
		RYUJIN_KEY_THOUSANDSSEPARATOR,
		RYUJIN_KEY_DECIMALSEPARATOR,
		RYUJIN_KEY_CURRENCYUNIT,
		RYUJIN_KEY_CURRENCYSUBUNIT,
		RYUJIN_KEY_KP_LEFTPAREN,
		RYUJIN_KEY_KP_RIGHTPAREN,
		RYUJIN_KEY_KP_LEFTBRACE,
		RYUJIN_KEY_KP_RIGHTBRACE,
		RYUJIN_KEY_KP_TAB,
		RYUJIN_KEY_KP_BACKSPACE,
		RYUJIN_KEY_KP_A,
		RYUJIN_KEY_KP_B,
		RYUJIN_KEY_KP_C,
		RYUJIN_KEY_KP_D,
		RYUJIN_KEY_KP_E,
		RYUJIN_KEY_KP_F,
		RYUJIN_KEY_KP_XOR,
		RYUJIN_KEY_KP_POWER,
		RYUJIN_KEY_KP_PERCENT,
		RYUJIN_KEY_KP_LESS,
		RYUJIN_KEY_KP_GREATER,
		RYUJIN_KEY_KP_AMPERSAND,
		RYUJIN_KEY_KP_DBLAMPERSAND,
		RYUJIN_KEY_KP_VERTICALBAR,
		RYUJIN_KEY_KP_DBLVERTICALBAR,
		RYUJIN_KEY_KP_COLON,
		RYUJIN_KEY_KP_HASH,
		RYUJIN_KEY_KP_SPACE,
		RYUJIN_KEY_KP_AT,
		RYUJIN_KEY_KP_EXCLAM,
		RYUJIN_KEY_KP_MEMSTORE,
		RYUJIN_KEY_KP_MEMRECALL,
		RYUJIN_KEY_KP_MEMCLEAR,
		RYUJIN_KEY_KP_MEMADD,
		RYUJIN_KEY_KP_MEMSUBTRACT,
		RYUJIN_KEY_KP_MEMMULTIPLY,
		RYUJIN_KEY_KP_MEMDIVIDE,
		RYUJIN_KEY_KP_PLUSMINUS,
		RYUJIN_KEY_KP_CLEAR,
		RYUJIN_KEY_KP_CLEARENTRY,
		RYUJIN_KEY_KP_BINARY,
		RYUJIN_KEY_KP_OCTAL,
		RYUJIN_KEY_KP_DECIMAL,
		RYUJIN_KEY_KP_HEXADECIMAL,
		0, 0,
		RYUJIN_KEY_LCTRL,
		RYUJIN_KEY_LSHIFT,
		RYUJIN_KEY_LALT,
		RYUJIN_KEY_LGUI,
		RYUJIN_KEY_RCTRL,
		RYUJIN_KEY_RSHIFT,
		RYUJIN_KEY_RALT,
		RYUJIN_KEY_RGUI,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		RYUJIN_KEY_MODE,
		RYUJIN_KEY_AUDIONEXT,
		RYUJIN_KEY_AUDIOPREV,
		RYUJIN_KEY_AUDIOSTOP,
		RYUJIN_KEY_AUDIOPLAY,
		RYUJIN_KEY_AUDIOMUTE,
		RYUJIN_KEY_MEDIASELECT,
		RYUJIN_KEY_WWW,
		RYUJIN_KEY_MAIL,
		RYUJIN_KEY_CALCULATOR,
		RYUJIN_KEY_COMPUTER,
		RYUJIN_KEY_AC_SEARCH,
		RYUJIN_KEY_AC_HOME,
		RYUJIN_KEY_AC_BACK,
		RYUJIN_KEY_AC_FORWARD,
		RYUJIN_KEY_AC_STOP,
		RYUJIN_KEY_AC_REFRESH,
		RYUJIN_KEY_AC_BOOKMARKS,
		RYUJIN_KEY_BRIGHTNESSDOWN,
		RYUJIN_KEY_BRIGHTNESSUP,
		RYUJIN_KEY_DISPLAYSWITCH,
		RYUJIN_KEY_KBDILLUMTOGGLE,
		RYUJIN_KEY_KBDILLUMDOWN,
		RYUJIN_KEY_KBDILLUMUP,
		RYUJIN_KEY_EJECT,
		RYUJIN_KEY_SLEEP,
	};
}


#endif
