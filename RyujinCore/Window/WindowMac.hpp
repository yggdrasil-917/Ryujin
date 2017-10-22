#ifndef WINDOW_MAC_HPP
#define WINDOW_MAC_HPP


#include "Window.hpp"

#if PLATFORM_MAC

#define RYUJIN_FULLSCREEN_WINDOW_STYLE	0
#define RYUJIN_WINDOW_STYLE             (NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable)
#define RYUJIN_BORDERLESS_STYLE			0
#define RYUJIN_WINDOW_EX				0


@interface RyujinWindow : NSWindow
@property (nonatomic, readonly) id<MTLDevice> device;

- (void)CacheDevice:(id<MTLDevice>) inDevice;

- (void)mouseMoved:(NSEvent *)theEvent;

- (BOOL)acceptsFirstResponder;
- (void)keyDown:(NSEvent *)theEvent;

@end

@interface RyujinSplashWindow : NSWindow

@end

namespace Ryujin
{
    class WindowMac : public Window
    {
    protected:
        bool BuildNormalWindow(const WindowDescriptor& winDesc) OVERRIDE FINAL;
        bool BuildSplashScreen(const WindowDescriptor& winDesc) OVERRIDE FINAL;
        
    public:
        WindowMac();
        
        bool BuildWindow(const WindowDescriptor& winDesc) OVERRIDE FINAL;
        void DestroyWindow() OVERRIDE FINAL;
        void Resize(const uint32 inWidth, const uint32 inHeight) OVERRIDE FINAL;
//        void AttachRenderingContext(PlatformDevicePtr device) OVERRIDE FINAL;
        void ToggleFullscreen(bool bEnableFullscreen) OVERRIDE FINAL;
        void SetWindowVisibility(bool bShow) OVERRIDE FINAL;
        
        void SetVSync(const int32 vsync) OVERRIDE FINAL;
    };
}
#endif // PLATFORM_MAC


#endif
