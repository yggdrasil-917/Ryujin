

#ifndef WindowDelegate_h
#define WindowDelegate_h

#include <Cocoa/Cocoa.h>

@interface RyujinWindowDelegate : NSObject<NSWindowDelegate>
    
- (void)windowDidResize:(NSNotification *)notification;
    
- (void)windowWillClose:(NSNotification *)notification;
@end


#endif 