


#include "Events.hpp"

#if PLATFORM_WINDOWS

#include "Mouse.hpp"
#include <windowsx.h>
#include "Ryujin/resource.h"

using namespace Ryujin;

/* Windows scancode to Ryujin scancode mapping table */
/* derived from Microsoft scan code document, http://download.microsoft.com/download/1/6/1/161ba512-40e2-4cc9-843a-923143f3456c/scancode.doc */

static const RyujinScanCode windowsScanCodeTable[] =
{
	/*	0						1							2							3							4						5							6							7 */
	/*	8						9							A							B							C						D							E							F */
	RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_ESCAPE,		RYUJIN_SCANCODE_1,			RYUJIN_SCANCODE_2,			RYUJIN_SCANCODE_3,		RYUJIN_SCANCODE_4,			RYUJIN_SCANCODE_5,			RYUJIN_SCANCODE_6,			/* 0 */
	RYUJIN_SCANCODE_7,			RYUJIN_SCANCODE_8,			RYUJIN_SCANCODE_9,			RYUJIN_SCANCODE_0,			RYUJIN_SCANCODE_MINUS,	RYUJIN_SCANCODE_EQUALS,		RYUJIN_SCANCODE_BACKSPACE,	RYUJIN_SCANCODE_TAB,		/* 0 */

	RYUJIN_SCANCODE_Q,			RYUJIN_SCANCODE_W,			RYUJIN_SCANCODE_E,			RYUJIN_SCANCODE_R,			RYUJIN_SCANCODE_T,		RYUJIN_SCANCODE_Y,			RYUJIN_SCANCODE_U,			RYUJIN_SCANCODE_I,			/* 1 */
	RYUJIN_SCANCODE_O,			RYUJIN_SCANCODE_P,			RYUJIN_SCANCODE_LEFTBRACKET,	RYUJIN_SCANCODE_RIGHTBRACKET,	RYUJIN_SCANCODE_RETURN,	RYUJIN_SCANCODE_LCTRL,		RYUJIN_SCANCODE_A,			RYUJIN_SCANCODE_S,			/* 1 */

	RYUJIN_SCANCODE_D,			RYUJIN_SCANCODE_F,			RYUJIN_SCANCODE_G,			RYUJIN_SCANCODE_H,			RYUJIN_SCANCODE_J,		RYUJIN_SCANCODE_K,			RYUJIN_SCANCODE_L,			RYUJIN_SCANCODE_SEMICOLON,	/* 2 */
	RYUJIN_SCANCODE_APOSTROPHE,	RYUJIN_SCANCODE_GRAVE,		RYUJIN_SCANCODE_LSHIFT,		RYUJIN_SCANCODE_BACKSLASH,	RYUJIN_SCANCODE_Z,		RYUJIN_SCANCODE_X,			RYUJIN_SCANCODE_C,			RYUJIN_SCANCODE_V,			/* 2 */

	RYUJIN_SCANCODE_B,			RYUJIN_SCANCODE_N,			RYUJIN_SCANCODE_M,			RYUJIN_SCANCODE_COMMA,		RYUJIN_SCANCODE_PERIOD,	RYUJIN_SCANCODE_SLASH,		RYUJIN_SCANCODE_RSHIFT,		RYUJIN_SCANCODE_PRINTSCREEN,/* 3 */
	RYUJIN_SCANCODE_LALT,			RYUJIN_SCANCODE_SPACE,		RYUJIN_SCANCODE_CAPSLOCK,		RYUJIN_SCANCODE_F1,			RYUJIN_SCANCODE_F2,		RYUJIN_SCANCODE_F3,			RYUJIN_SCANCODE_F4,			RYUJIN_SCANCODE_F5,		/* 3 */

	RYUJIN_SCANCODE_F6,			RYUJIN_SCANCODE_F7,			RYUJIN_SCANCODE_F8,			RYUJIN_SCANCODE_F9,			RYUJIN_SCANCODE_F10,		RYUJIN_SCANCODE_NUMLOCKCLEAR,	RYUJIN_SCANCODE_SCROLLLOCK,	RYUJIN_SCANCODE_HOME,		/* 4 */
	RYUJIN_SCANCODE_UP,			RYUJIN_SCANCODE_PAGEUP,		RYUJIN_SCANCODE_KP_MINUS,		RYUJIN_SCANCODE_LEFT,			RYUJIN_SCANCODE_KP_5,		RYUJIN_SCANCODE_RIGHT,		RYUJIN_SCANCODE_KP_PLUS,		RYUJIN_SCANCODE_END,		/* 4 */

	RYUJIN_SCANCODE_DOWN,			RYUJIN_SCANCODE_PAGEDOWN,		RYUJIN_SCANCODE_INSERT,		RYUJIN_SCANCODE_DELETE,		RYUJIN_SCANCODE_UNKNOWN,	RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_NONUSBACKSLASH,RYUJIN_SCANCODE_F11,		/* 5 */
	RYUJIN_SCANCODE_F12,			RYUJIN_SCANCODE_PAUSE,		RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_LGUI,			RYUJIN_SCANCODE_RGUI,		RYUJIN_SCANCODE_APPLICATION,	RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,	/* 5 */

	RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_F13,		RYUJIN_SCANCODE_F14,			RYUJIN_SCANCODE_F15,			RYUJIN_SCANCODE_F16,		/* 6 */
	RYUJIN_SCANCODE_F17,			RYUJIN_SCANCODE_F18,			RYUJIN_SCANCODE_F19,			RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,	RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,	/* 6 */

	RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,	RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,	/* 7 */
	RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,	RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN,		RYUJIN_SCANCODE_UNKNOWN	/* 7 */
};

static RyujinScanCode WindowsScanCodeToRyujinScanCode(LPARAM lParam, WPARAM wParam)
{
	RyujinScanCode code;
	int32 scanCode = (lParam >> 16) & 0xFF;

	if (scanCode > 127)
		return RYUJIN_SCANCODE_UNKNOWN;

	code = windowsScanCodeTable[scanCode];

	return code;
}

static void UpdateMouseButtonStates(WPARAM wParam)
{
	Mouse* mouse = Mouse::Instance();
	if (wParam == mouse->GetButtonFlags())
		return;

	mouse->UpdateMouseButtonState(MouseButton::LButton, (wParam & MK_LBUTTON) ? RyujinState::Pressed : RyujinState::Released);
	mouse->UpdateMouseButtonState(MouseButton::MButton, (wParam & MK_MBUTTON) ? RyujinState::Pressed : RyujinState::Released);
	mouse->UpdateMouseButtonState(MouseButton::RButton, (wParam & MK_RBUTTON) ? RyujinState::Pressed : RyujinState::Released);

	mouse->CacheMouseButtonState(wParam);
}

LRESULT CALLBACK Ryujin::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int32 width = 0;
	static int32 height = 0;

	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_ACTIVE)
		{
			PushWindowEvent(WindowEventID::FocusGained);
		}
		else
		{
			PushWindowEvent(WindowEventID::FocusLost);
		}
		break;

	case WM_MOUSEMOVE:
		// don't break here so it falls through to be able to process mouse button presses
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(hwnd, &p);
		int32 x = p.x;
		int32 y = p.y;
		if (Mouse::Instance()->UpdateCursorLocation(x, y))
		{
			PushEvent(EngineEvent::MouseMotion, WindowEventID::None, x, y);
		}
	}

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_XBUTTONDOWN:
		UpdateMouseButtonStates(wParam);
		break;

	case WM_MOUSEWHEEL:
		{
			int32 wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			Mouse::Instance()->UpdateWheelDelta(wheelDelta);
		}
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			int32 scanCode = (lParam >> 16) & 0xFF;

			if (scanCode < sizeof(windowsScanCodeTable) / sizeof(windowsScanCodeTable[0]))
			{
				RyujinScanCode RyujinScanCode = windowsScanCodeTable[scanCode];
				Keyboard::Instance()->SetKeyState(RyujinScanCode, KEY_PRESSED);
				PushKeyboardEvent(EngineEvent::KeyDown, Keyboard::Instance()->GetKeyMap(RyujinScanCode), Keyboard::Instance()->GetModState());
			}
		}
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
			int32 scanCode = (lParam >> 16) & 0xFF;

			if (scanCode >= sizeof(windowsScanCodeTable) / sizeof(windowsScanCodeTable[0]))
			{
				break;
			}

			RyujinScanCode RyujinScanCode = windowsScanCodeTable[scanCode];
			Keyboard::Instance()->SetKeyState(RyujinScanCode, KEY_RELEASED);
			PushKeyboardEvent(EngineEvent::KeyUp, Keyboard::Instance()->GetKeyMap(RyujinScanCode), Keyboard::Instance()->GetModState());

			const uint8* keyState = Keyboard::Instance()->GetKeyboardState();
			if (keyState[RYUJIN_SCANCODE_LALT] || keyState[RYUJIN_SCANCODE_RALT])
			{
				if (RyujinScanCode == RYUJIN_SCANCODE_F4)
					PushWindowEvent(WindowEventID::Close);
			}
		}
		break;

	case WM_STYLECHANGED:
		// Do nothing here (solves a problem with toggling fullscreen receiving the incorrect dimensions)
		break;

	case WM_SIZE:
		width = LOWORD(lParam);
		height = HIWORD(lParam);

		switch (wParam)
		{
		case SIZE_MAXIMIZED:
			PushWindowEvent(WindowEventID::Resize, width, height);
			break;
			
		case SIZE_MINIMIZED:
			break;

		default: // Restore
			PushWindowEvent(WindowEventID::Resize, width, height);
			break;
		}

		break;

	case WM_EXITSIZEMOVE:
		PushWindowEvent(WindowEventID::Resize, width, height); 
		break;

	case WM_CLOSE:
		{
			//PostQuitMessage(0);
			PushEvent(EngineEvent::Quit);
		}
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

HBITMAP splash;
LRESULT CALLBACK Ryujin::SplashWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_CREATE:
		splash = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(RYUJIN_SPLASH));
		break;

	case WM_PAINT:
	{
		BITMAP bm;
		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hwnd, &ps);

		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, splash);

		GetObject(splash, sizeof(bm), &bm);

		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hbmOld);
		DeleteDC(hdcMem);

		EndPaint(hwnd, &ps);
	}
	break;

	case WM_DESTROY:
		DeleteObject(splash);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

void Ryujin::PumpEvents_Windows()
{
	MSG msg;
	DWORD startTicks = (DWORD)GetTickCount64();

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	{
		if (msg.message == WM_QUIT)               
		{
			PushEvent(EngineEvent::Quit);
			break;
		}
		else                            
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (int32(startTicks - msg.time) <= 0)
		{
			break; // if there are too many messages to process then break after a while
		}
	}
}


#endif // PLATFORM_WINDOWS