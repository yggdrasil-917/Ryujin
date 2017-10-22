#pragma once

#include "../Logger/Logger.hpp"
#include "../Containers/String.hpp"

#if PLATFORM_LINUX
#include <xcb/xcb.h>
#endif

namespace Ryujin
{
	struct WindowDescriptor
	{
		int32 posX;
		int32 posY;
		int32 width;
		int32 height;
		ubyte colorBits;
		ubyte depthBits;
		ubyte stencilBits;
		uint16 glVersions;
		bool bIsFullscreen;
        bool bIsSplashScreen;
		bool bShowMinimized;
        String windowTitleOrSplashScreenPath;

		WindowDescriptor() :
			posX(0),
			posY(0),
			width(1280),
			height(720),
			bIsFullscreen(false),
            bIsSplashScreen(false),
			bShowMinimized(false)
		{}
	};

#if PLATFORM_WINDOWS
	using WindowHandle = HWND;
	using RenderContext = HGLRC;
	using DeviceContext = HDC;
#elif PLATFORM_MAC
    using WindowHandle = void*;
    using RenderContext = uint32;
    using DeviceContext = uint32;
#elif PLATFORM_LINUX
    using WindowHandle = xcb_window_t;
    using RenderContext = xcb_connection_t*;
    using DeviceContext = xcb_screen_t*;
#else
    #error "Platform does not define window types"
#endif

	class CORE_API Window
	{
	protected:
		uint32 width;
		uint32 height;
		bool bIsFullscreen;
        bool bIsSplashScreen;
		WindowHandle windowHandle;
		RenderContext renderContext;
		DeviceContext deviceContext;
        
        VIRTUAL bool BuildNormalWindow(const WindowDescriptor& winDesc) = 0;
        VIRTUAL bool BuildSplashScreen(const WindowDescriptor& winDesc) = 0;

	public:
		Window() {}
		VIRTUAL ~Window() {}

		VIRTUAL bool BuildWindow(const WindowDescriptor& winDesc) = 0;
		VIRTUAL void DestroyWindow() = 0;
        VIRTUAL void Resize(const uint32 inWidth, const uint32 inHeight) = 0;
		VIRTUAL void ToggleFullscreen(bool bEnableFullscreen) = 0;
		VIRTUAL void SetWindowVisibility(bool bShow) = 0;
        
		VIRTUAL void SetVSync(const int32 vsync) = 0;

		FORCEINLINE uint32 GetWidth() const { return width; }
		FORCEINLINE uint32 GetHeight() const { return height; }
		FORCEINLINE bool IsFullscreen() const { return bIsFullscreen; }
		FORCEINLINE WindowHandle GetWindowHandle() const { return windowHandle; }
		FORCEINLINE DeviceContext GetDeviceContext() const { return deviceContext; }
        FORCEINLINE RenderContext GetRenderContext() const { return renderContext; }
	};
}


