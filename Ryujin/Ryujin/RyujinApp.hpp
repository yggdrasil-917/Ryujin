

#include "Engine.hpp"

#if PLATFORM_MAC
#import <Cocoa/Cocoa.h>

@interface RyujinAppDelegate : NSObject <NSApplicationDelegate>


@end


@interface RyujinApplication : NSApplication
{
    bool bShouldKeepRunning;
}

-(void)run;
-(void)terminate:(id)sender;
@end



namespace Ryujin
{
    void RunApplication(Engine& engine, int argc, char **argv);
}
#endif