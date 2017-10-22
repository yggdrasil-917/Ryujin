#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "../CoreUtils/ConfigCache.hpp"

#include "../Input/Events.hpp"
#include "../Input/Mouse.hpp"
#include "Window.hpp"



namespace Ryujin
{
	class CORE_API Viewport
	{
	private:
		uint32 clientWidth;
		uint32 clientHeight;
		bool fullscreen;

		int32 vSync;

		Window* window;

	public:

#if PLATFORM_WINDOWS
		#ifdef CreateWindow
		#undef CreateWindow
		#endif
#endif

		Viewport();
		VIRTUAL ~Viewport();

        bool CreateWindow(const WindowDescriptor& desc);
		void CheckFullscreenChange(bool inFullscreen);
		VIRTUAL void WarpMouse(uint32 x, uint32 y);
		VIRTUAL void SetMouseCursorVisible(bool bIsVisible);
		VIRTUAL void Present();
		VIRTUAL void Close();

        FORCEINLINE Window* GetWindow() const { return window; }
		FORCEINLINE void GetClientDimensions(uint32 &width, uint32 &height) const { width = clientWidth; height = clientHeight; }

		FORCEINLINE void Resize(const uint32 width, const uint32 height)
		{ 
			clientWidth = width;
			clientHeight = height;
            
            window->Resize(width, height);
		}

		FORCEINLINE bool IsFullscreen() const { return fullscreen; }
		FORCEINLINE bool IsUsingVSync() const { return vSync != 0; }
		
		FORCEINLINE void SetVsync(int32 interval)
		{ 
			vSync = interval;
			window->SetVSync(vSync);
		}

		FORCEINLINE void SetFullscreen(bool enable) 
		{
			window->ToggleFullscreen(enable);
			fullscreen = enable; 
		}
	};
}


#endif