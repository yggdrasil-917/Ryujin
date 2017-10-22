

#import "RyujinApp.hpp"
#include "RyujinCore/Window/WindowMac.hpp"
#include "RyujinCore/Input/Events.hpp"\


@interface RyujinAppDelegate ()

@end

@implementation RyujinAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
    PushEvent(Ryujin::EngineEvent::Quit);
}

- (void)applicationDidBecomeActive:(NSNotification *)aNotification;
{
    PushWindowEvent(Ryujin::WindowEventID::FocusGained);
}

- (void)applicationDidResignActive:(NSNotification *)aNotification
{
    PushWindowEvent(Ryujin::WindowEventID::FocusLost);
}

@end




@implementation RyujinApplication

- (void)run
{
    [[NSNotificationCenter defaultCenter]
     postNotificationName:NSApplicationWillFinishLaunchingNotification
     object:NSApp];
    [[NSNotificationCenter defaultCenter]
     postNotificationName:NSApplicationDidFinishLaunchingNotification
     object:NSApp];
    
    bShouldKeepRunning = YES;
    do
    {
        NSEvent *event =
        [self
         nextEventMatchingMask:NSEventMaskAny
         untilDate:[NSDate distantPast]
         inMode:NSDefaultRunLoopMode
         dequeue:YES];
        
        [self sendEvent:event];
        [self updateWindows];
    } while (bShouldKeepRunning);
}

- (void)terminate:(id)sender
{
    bShouldKeepRunning = NO;
}

@end




namespace Ryujin
{
    void RunApplication(Engine& engine, int argc, char **argv)
    {
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        
        [RyujinApplication sharedApplication];
        RyujinAppDelegate *appDelegate = [[[RyujinAppDelegate alloc] init] autorelease];
        [NSApp setDelegate:appDelegate];
        
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        id menubar = [[NSMenu new] autorelease];
        id appMenuItem = [[NSMenuItem new] autorelease];
        [menubar addItem:appMenuItem];
        [NSApp setMainMenu:menubar];
        id appMenu = [[NSMenu new] autorelease];
        id appName = [[NSProcessInfo processInfo] processName];
        id quitTitle = [@"Quit " stringByAppendingString:appName];
        id quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle
                                                      action:@selector(terminate:) keyEquivalent:@"q"];
        [appMenu addItem:quitMenuItem];
        [appMenuItem setSubmenu:appMenu];
        
        engine.Run(argc, argv);
        
        [pool release];
    }
    
    void Engine::ShowSplash()
    {
        WindowDescriptor splash;
        splash.width = 400;
        splash.height = 600;
        splash.bIsSplashScreen = true;
        splash.windowTitleOrSplashScreenPath = String::Printf("%sSplash/Ryujin.jpg", AppInfo::GetResourcesDir());
        
        splashScreen = RYUJIN_NEW WindowMac();
        splashScreen->BuildWindow(splash);
        
        PumpEvents_Mac();
    }
    
    void Engine::HideSplash()
    {
        if (splashScreen)
        {
            splashScreen->DestroyWindow();
            SAFE_DELETE(splashScreen);
        }
    }
}




