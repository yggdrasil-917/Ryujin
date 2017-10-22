


#include "WindowMac.hpp"
#include "../Input/Events.hpp"
#include "../Input/Mouse.hpp"
#include "WindowDelegate.hpp"

#import <QuartzCore/CAMetalLayer.h>

@implementation RyujinWindow
@synthesize device;

- (void)CacheDevice:(id<MTLDevice>) inDevice;
{
    device = inDevice;
}

- (void)mouseMoved:(NSEvent *)theEvent
{
    NSView* view = [self contentView];
    NSRect frame = [view frame];
    // NOTE: locationInView still needs to be clamped to window dimensions
    NSPoint locationInView = [view convertPoint:[theEvent locationInWindow] fromView:nil];
    locationInView.x = locationInView.x < 0.0f ? 0.0f : (locationInView.x > frame.size.width ? frame.size.width : locationInView.x);
    locationInView.y = locationInView.y < 0.0f ? 0.0f : (locationInView.y > frame.size.height ? frame.size.height : locationInView.y);
    //NSLog(@"Mouse %f %f", locationInView.x, locationInView.y);
    Ryujin::int32 x = (Ryujin::int32)locationInView.x;
    Ryujin::int32 y = (Ryujin::int32)locationInView.y;
    if (Ryujin::Mouse::Instance()->UpdateCursorLocation(x, y))
    {
//        NSLog(@"%d %d", x, y);
        PushEvent(Ryujin::EngineEvent::MouseMotion, Ryujin::WindowEventID::None, x, y);
    }
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)keyDown:(NSEvent *)theEvent
{
    // this eliminates the stupid beeping sound
}

@end




@implementation RyujinSplashWindow

- (id)initWithContentRect:(NSRect)contentRect
                styleMask:(NSUInteger)aStyle
                  backing:(NSBackingStoreType)bufferingType
                    defer:(BOOL)flag
{
    RyujinSplashWindow* window = [super initWithContentRect:contentRect
                                                  styleMask:NSWindowStyleMaskBorderless
                                                    backing:NSBackingStoreBuffered defer:NO];
    
    window.appearance = [NSAppearance appearanceNamed:NSAppearanceNameVibrantDark];
    window.styleMask = window.styleMask | NSWindowStyleMaskBorderless;
    window.titleVisibility = NSWindowTitleHidden;
    window.titlebarAppearsTransparent = YES;
    window.movableByWindowBackground = NO;
    window.level = NSModalPanelWindowLevel;
    window.alphaValue = 1.0;
    window.backgroundColor = [NSColor blackColor];
    window.hasShadow = YES;
    return window;
}

@end



namespace Ryujin
{
    WindowMac::WindowMac()
    :
    Window()
    {
        windowHandle = nullptr;
    }
    
    bool WindowMac::BuildNormalWindow(const WindowDescriptor &winDesc)
    {
        width = winDesc.width;
        height = winDesc.height;
        bIsFullscreen = winDesc.bIsFullscreen;
        bIsSplashScreen = false;
        renderContext = 0;
        deviceContext = 0;
        RyujinWindow* nsWindow = [[RyujinWindow alloc]
                                  initWithContentRect: NSMakeRect(winDesc.posX, winDesc.posY, winDesc.width, winDesc.height)
                                  styleMask: RYUJIN_WINDOW_STYLE
                                  backing: NSBackingStoreBuffered
                                  defer: NO];
        
        if (!nsWindow)
        {
            return false;
        }
        
        RyujinWindowDelegate* ryujinWindowDelegate = [[[RyujinWindowDelegate alloc] init] autorelease];
        [nsWindow setDelegate: ryujinWindowDelegate];
        [nsWindow makeFirstResponder:nsWindow];
        [nsWindow setAcceptsMouseMovedEvents:true];
        [[nsWindow contentView] setWantsLayer:YES];
        
        [nsWindow setTitle: [NSString stringWithUTF8String:*winDesc.windowTitleOrSplashScreenPath]];
        [nsWindow center];
        [nsWindow makeKeyAndOrderFront:nil];
        windowHandle = (WindowHandle)nsWindow;
        
        return true;
    }
    
    bool WindowMac::BuildSplashScreen(const WindowDescriptor &winDesc)
    {
        NSImage* image = [[[NSImage alloc]initWithContentsOfFile:[NSString stringWithUTF8String:*winDesc.windowTitleOrSplashScreenPath]] autorelease];
        if (image == nil)
        {
            NSLog(@"Nil splash screen image");
            return false;
        }
        
        width = image.size.width; // winDesc.width;
        height = image.size.height; // winDesc.height;
        bIsFullscreen = false;
        bIsSplashScreen = true;
        renderContext = 0;
        deviceContext = 0;
        RyujinSplashWindow* nsWindow = [[RyujinSplashWindow alloc]
                                        initWithContentRect: NSMakeRect(winDesc.posX, winDesc.posY, winDesc.width, winDesc.height)
                                        styleMask: 0
                                        backing: NSBackingStoreBuffered
                                        defer: NO];
        
        if (!nsWindow)
        {
            return false;
        }
        
        
        [nsWindow setBackgroundColor:[NSColor colorWithPatternImage:image]];
        [nsWindow center];
        [nsWindow makeKeyAndOrderFront:nil];
        windowHandle = (WindowHandle)nsWindow;
        
        return true;
    }
    
    bool WindowMac::BuildWindow(const WindowDescriptor& winDesc)
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
    
    void WindowMac::DestroyWindow()
    {
        if (bIsSplashScreen)
        {
            RyujinSplashWindow* nsWindow = (RyujinSplashWindow*)windowHandle;
            [nsWindow release];
        }
        else
        {
            RyujinWindow* nsWindow = (RyujinWindow*)windowHandle;
            [nsWindow release];
        }
        windowHandle = nullptr;
    }
    
    void WindowMac::Resize(const uint32 inWidth, const uint32 inHeight)
    {
        if (!bIsSplashScreen)
        {
            RyujinWindow* ryujinWindow = (RyujinWindow*)windowHandle;
            width = (uint32)[ryujinWindow frame].size.width;
            height = (uint32)[ryujinWindow frame].size.height;
        }
    }
    
    void WindowMac::ToggleFullscreen(bool bEnableFullscreen)
    {
        
    }
    
    void WindowMac::SetWindowVisibility(bool bShow)
    {
        
    }
    
    void WindowMac::SetVSync(const int32 vsync)
    {
        
    }
}
