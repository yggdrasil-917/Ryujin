

#include "WindowWindows.hpp"
#include "Viewport.hpp"
#include "../Input/Events.hpp"
#include "Ryujin/resource.h"




Ryujin::WindowWindows::~WindowWindows()
{
	DestroyWindow();
}

bool Ryujin::WindowWindows::BuildNormalWindow(const WindowDescriptor& winDesc)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = TEXT("RyujinEngineClass");
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(RYUJIN_ICON));

	if (!RegisterClass(&wc))
		return false;

	int32 screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int32 screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect;
	windowRect.left = winDesc.posX;
	windowRect.right = winDesc.width;
	windowRect.bottom = winDesc.height;
	windowRect.top = winDesc.posY;

	uint32 windowFlags = RYUJIN_WINDOW_STYLE;
	uint32 windowExFlags = RYUJIN_WINDOW_EX;
	if (winDesc.bIsFullscreen)
	{
		// Right now default to native res in fullscreen
		windowFlags = RYUJIN_FULLSCREEN_WINDOW_STYLE;
		HMONITOR hmon = MonitorFromWindow(windowHandle, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		if (!GetMonitorInfo(hmon, &mi))
			return false;

		windowRect.left = mi.rcMonitor.left;
		windowRect.right = mi.rcMonitor.right;
		windowRect.top = mi.rcMonitor.top;
		windowRect.bottom = mi.rcMonitor.bottom;
	}
	AdjustWindowRectEx(&windowRect, windowFlags, FALSE, windowExFlags);
	width = windowRect.right - windowRect.left;
	height = windowRect.bottom - windowRect.top;

	// Clamp window dimensions in case it is bigger than monitor screen resolution
	width = MathUtils::Min<uint32>(width, screenWidth);
	height = MathUtils::Min<uint32>(height, screenHeight);

	HINSTANCE hInst = GetModuleHandle(NULL);

	windowHandle = CreateWindowEx(windowExFlags, wc.lpszClassName,
		TEXT("Ryujin Engine"),
		windowFlags,
		(screenWidth - width) / 2, (screenHeight - height) / 2, width, height,
		NULL,
		NULL, hInst, NULL);

	if (!windowHandle)
	{
		return false;
	}

	RECT clientDimensions;
	GetClientRect(windowHandle, &clientDimensions);

	ShowWindow(windowHandle, winDesc.bShowMinimized ? SW_SHOWMINIMIZED : SW_SHOW);

	deviceContext = GetDC(windowHandle);

	DWORD pfdFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		1,                     // version number 
		pfdFlags,      // double buffered 
		PFD_TYPE_RGBA,         // RGBA type 
		winDesc.colorBits,  // 24-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		0,                     // no alpha buffer 
		0,                     // shift bit ignored 
		0,                     // no accumulation buffer 
		0, 0, 0, 0,            // accum bits ignored 
		winDesc.depthBits,           // 24-bit z-buffer 
		winDesc.stencilBits,          // stencil buffer 
		0,                     // no auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer     0,                     // reserved 
		0, 0, 0                // layer masks ignored
	};

	int32 pixelFormat = ChoosePixelFormat(deviceContext, &pfd);

	if (!SetPixelFormat(deviceContext, pixelFormat, &pfd))
		return false;

	return true;
}

bool Ryujin::WindowWindows::BuildSplashScreen(const WindowDescriptor& winDesc)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = SplashWindowProc;
	wc.hInstance = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = TEXT("RyujinSplashClass");
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(RYUJIN_ICON));

	if (!RegisterClass(&wc))
		return false;

	int32 screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int32 screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect;
	windowRect.left = winDesc.posX;
	windowRect.right = winDesc.width;
	windowRect.bottom = winDesc.height;
	windowRect.top = winDesc.posY;

	uint32 windowFlags = RYUJIN_BORDERLESS_STYLE;
	uint32 windowExFlags = RYUJIN_WINDOW_EX;
	AdjustWindowRectEx(&windowRect, windowFlags, FALSE, windowExFlags);
	width = windowRect.right - windowRect.left;
	height = windowRect.bottom - windowRect.top;

	// Clamp window dimensions in case it is bigger than monitor screen resolution
	width = MathUtils::Min<uint32>(width, screenWidth);
	height = MathUtils::Min<uint32>(height, screenHeight);

	HINSTANCE hInst = GetModuleHandle(NULL);

	windowHandle = CreateWindowEx(windowExFlags, wc.lpszClassName,
		TEXT("Ryujin Engine"),
		windowFlags,
		(screenWidth - width) / 2, (screenHeight - height) / 2, width, height,
		NULL,
		NULL, hInst, NULL);

	if (!windowHandle)
	{
		return false;
	}

	RECT clientDimensions;
	GetClientRect(windowHandle, &clientDimensions);

	ShowWindow(windowHandle, SW_SHOW);

	deviceContext = GetDC(windowHandle);

	DWORD pfdFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		1,                     // version number 
		pfdFlags,      // double buffered 
		PFD_TYPE_RGBA,         // RGBA type 
		winDesc.colorBits,  // 24-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		0,                     // no alpha buffer 
		0,                     // shift bit ignored 
		0,                     // no accumulation buffer 
		0, 0, 0, 0,            // accum bits ignored 
		winDesc.depthBits,           // 24-bit z-buffer 
		winDesc.stencilBits,          // stencil buffer 
		0,                     // no auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer     0,                     // reserved 
		0, 0, 0                // layer masks ignored
	};

	int32 pixelFormat = ChoosePixelFormat(deviceContext, &pfd);

	if (!SetPixelFormat(deviceContext, pixelFormat, &pfd))
		return false;

	return true;
}

bool Ryujin::WindowWindows::BuildWindow(const WindowDescriptor& winDesc)
{
	if (winDesc.bIsSplashScreen)
	{
		return BuildSplashScreen(winDesc);
	}
	else
	{
		return BuildNormalWindow(winDesc);
	}
}


void Ryujin::WindowWindows::DestroyWindow()
{
	::DestroyWindow(windowHandle);
}


void Ryujin::WindowWindows::Resize(const uint32 inWidth, const uint32 inHeight)
{
    
}


void Ryujin::WindowWindows::ToggleFullscreen(bool bEnableFullscreen)
{
	bIsFullscreen = bEnableFullscreen;
	RECT windowRect;
	uint32 windowFlags = WS_VISIBLE;
	uint32 windowExFlags = 0;
	if (bEnableFullscreen)
	{
		windowFlags |= RYUJIN_FULLSCREEN_WINDOW_STYLE;
		windowExFlags = RYUJIN_WINDOW_EX;
		SetWindowLongPtr(windowHandle, GWL_STYLE, windowFlags);
		HMONITOR hmon = MonitorFromWindow(windowHandle, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		if (!GetMonitorInfo(hmon, &mi))
			return;

		windowRect.left = mi.rcMonitor.left;
		windowRect.right = mi.rcMonitor.right;
		windowRect.top = mi.rcMonitor.top;
		windowRect.bottom = mi.rcMonitor.bottom;

		AdjustWindowRectEx(&windowRect, windowFlags, FALSE, windowExFlags);
		SetWindowPos(windowHandle, HWND_TOPMOST, 0, 0, windowRect.right, windowRect.bottom, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSENDCHANGING);
	}
	else
	{
		windowFlags |= RYUJIN_WINDOW_STYLE;
		windowExFlags = RYUJIN_WINDOW_EX;
		int32 screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int32 screenHeight = GetSystemMetrics(SM_CYSCREEN);

		SetWindowLongPtr(windowHandle, GWL_STYLE, windowFlags);
		SetWindowPos(windowHandle, HWND_NOTOPMOST, (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSENDCHANGING);
	}
}

void Ryujin::WindowWindows::SetWindowVisibility(bool bShow)
{
	if (bShow)
	{
		ShowWindow(windowHandle, SW_RESTORE);
		SetForegroundWindow(windowHandle);
	}
	else
	{
		ShowWindow(windowHandle, SW_MINIMIZE);
	}
}


void Ryujin::WindowWindows::SetVSync(const int32 vsync)
{
}


