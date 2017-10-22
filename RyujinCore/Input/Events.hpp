#ifndef EVENTS_HPP
#define EVENTS_HPP


#include "../Logger/Logger.hpp"
#include "../CoreUtils/Singleton.hpp"
#include "Scancode.hpp"
#include "Keycode.hpp"
#include "Keyboard.hpp"
#include "../Containers/List.hpp"

#if PLATFORM_LINUX
#include <xcb/xcb.h>
#endif

namespace Ryujin
{
	enum class RyujinState : uint8
	{
		Released = 0,
		Pressed = 1,
	};

	enum class EngineEvent : uint8
	{
		None = 0,

		// User requested exit
		Quit = 0x10,
		ExitOnError,

		// Window events
		WindowEvent = 0x20,

		// Keyboard events
		KeyDown = 0x30,
		KeyUp,

		// Mouse events
		MouseMotion = 0x40,
		MouseButtonDown,
		MouseButtonUp,
		MouseWheel,
	};

	enum class WindowEventID : uint8
	{
		None = 0,
		Resize,
		FocusGained,
		FocusLost,
		Shown,
		Hidden,
		Moved,
		Minimized,
		Maximized,
		Close,
	};

	struct RyujinEvent
	{
		EngineEvent engineEvent;
		WindowEventID windowEventID;
		int32 Data1;
		int32 Data2;
	};
    

	class RyujinEventQueue : public Singleton<RyujinEventQueue>
	{
    public:
        // TODO: Profile this to see if I should switch over to a array instead of list
		List<RyujinEvent> events;
        RyujinEventQueue()
        {
            
        }
	};


#if PLATFORM_WINDOWS
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK SplashWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void CORE_API PumpEvents_Windows();
#elif PLATFORM_MAC
    void PumpEvents_Mac();
    void WarpMouseCocoa(void* windowHandle, uint32 x, uint32 y);
    void ShowCursorCocoa(bool bIsVisible);
#elif PLATFORM_LINUX
    void PumpEvents_Linux();
    void HandleEvent(const xcb_generic_event_t *event);
#endif

    extern CORE_API void PumpEvents();

    extern CORE_API bool PollEvents(RyujinEvent& inEvent);

    extern CORE_API void PushEvent(EngineEvent inEngineEvent, WindowEventID inWindowEvent = WindowEventID::None, int32 inData1 = 0, int32 inData2 = 0);

    extern CORE_API void PushKeyboardEvent(EngineEvent inEngineEvent, int32 key, KeyMod mod = KeyMod::None);

    extern CORE_API void PushWindowEvent(WindowEventID inWindowEvent, int32 inData1 = 0, int32 inData2 = 0);

    extern CORE_API void PopEvent();
}


#endif
