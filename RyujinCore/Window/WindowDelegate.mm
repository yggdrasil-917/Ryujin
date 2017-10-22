
#include "WindowDelegate.hpp"
#include "../Input/Events.hpp"


using namespace Ryujin;

@implementation RyujinWindowDelegate


- (void)windowDidResize:(NSNotification *)notification
{
    NSWindow* window = notification.object;
    NSRect size = [[window contentView] frame];
    PushWindowEvent(WindowEventID::Resize, (int32)size.size.width, (int32)size.size.height);
}

- (void)windowWillClose:(NSNotification *)notification
{
    PushWindowEvent(WindowEventID::Close);
}
@end

