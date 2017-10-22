#ifndef WINDOW_WINDOWS_HPP
#define WINDOW_WINDOWS_HPP


#include "Window.hpp"

#if PLATFORM_WINDOWS


#define RYUJIN_FULLSCREEN_WINDOW_STYLE	WS_POPUP
#define RYUJIN_WINDOW_STYLE				WS_OVERLAPPEDWINDOW
#define RYUJIN_BORDERLESS_STYLE			WS_POPUP
#define RYUJIN_WINDOW_EX				WS_EX_APPWINDOW

namespace Ryujin
{
	class CORE_API WindowWindows : public Window
	{
	private:
		bool BuildNormalWindow(const WindowDescriptor& winDesc) OVERRIDE FINAL;
		bool BuildSplashScreen(const WindowDescriptor& winDesc) OVERRIDE FINAL;

	public:
		~WindowWindows();

		bool BuildWindow(const WindowDescriptor& winDesc) OVERRIDE FINAL;
        void DestroyWindow() OVERRIDE FINAL;
        void Resize(const uint32 inWidth, const uint32 inHeight) OVERRIDE FINAL;
		void ToggleFullscreen(bool bEnableFullscreen) OVERRIDE FINAL;
		void SetWindowVisibility(bool bShow) OVERRIDE FINAL;

		void SetVSync(const int32 vsync) OVERRIDE FINAL;
	};
}
#endif // PLATFORM_WINDOWS


#endif