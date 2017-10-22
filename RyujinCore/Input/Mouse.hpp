#ifndef MOUSE_HPP
#define MOUSE_HPP


#include "../CoreUtils/Singleton.hpp"
#include "Events.hpp"

namespace Ryujin
{
#if PLATFORM_WINDOWS
	using MouseButtonFlags = WPARAM;
#else
	using MouseButtonFlags = uint64;
#endif

	enum MouseButton
	{
		LButton = 1,
		RButton,
		MButton,
		WheelUp,
		WheelDown,

		Count
	};

#define MOUSE_BUTTON(x) (1 << ((x) - MouseButton::LButton))
#define MOUSE_BUTTON_PRESSED(state, button) (state & MOUSE_BUTTON(button))

	class CORE_API Mouse// : public Singleton<Mouse>
	{
	private:
		int32 x;
		int32 y;
		int32 prevX;
		int32 prevY;
		MouseButtonFlags buttonFlags;
		RyujinState buttonState[MouseButton::Count];
		uint32 mouseButtonState;
		int32 wheelDelta;

	public:
		Mouse()
			: x(0), y(0), prevX(0), prevY(0)
		{}

		~Mouse()
		{}
        
        static Mouse* Instance();

		FORCEINLINE MouseButtonFlags GetButtonFlags() const { return buttonFlags; }
		FORCEINLINE RyujinState* GetButtonStates() { return &buttonState[0]; }

		FORCEINLINE bool UpdateCursorLocation(int32 newX, int32 newY)
		{
            if (newX == x && newY == y && prevX == x && prevY == y)
                return false;

			prevX = x;
			prevY = y;
			x = newX;
			y = newY;

			return true;
		}
        
        FORCEINLINE void WarpMouse(int32 newX, int32 newY)
        {
            prevX = newX;
            prevY = newY;
            x = newX;
            y = newY;
        }

		FORCEINLINE void GetMousePosition(int32& inX, int32& inY) const
		{
			inX = x;
			inY = y;
		}

		FORCEINLINE void GetCursorVector(int32& xDir, int32& yDir) 
		{
			xDir = x - prevX;
			yDir = y - prevY;
		}

		FORCEINLINE void CacheMouseButtonState(MouseButtonFlags inButtonFlags) { buttonFlags = inButtonFlags; }

		FORCEINLINE void UpdateMouseButtonState(MouseButton button, RyujinState state)
		{
			bool buttonWasPressed = false;
			if (MOUSE_BUTTON_PRESSED(mouseButtonState, button))
				buttonWasPressed = true;

			buttonState[button] = state;
			if (state == RyujinState::Pressed && !buttonWasPressed)
			{
				mouseButtonState |= MOUSE_BUTTON(button);
				PushEvent(EngineEvent::MouseButtonDown, WindowEventID::None, (int32)button);
			}
			else if (state == RyujinState::Released && buttonWasPressed)
			{
				mouseButtonState &= ~MOUSE_BUTTON(button);
				PushEvent(EngineEvent::MouseButtonUp, WindowEventID::None, (int32)button);
			}
		}

		FORCEINLINE uint32 GetCurrentButtonState() const { return mouseButtonState; }

		FORCEINLINE void UpdateWheelDelta(int32 inWheelDelta) 
		{
			wheelDelta = inWheelDelta; 
		}
	};
}


#endif
