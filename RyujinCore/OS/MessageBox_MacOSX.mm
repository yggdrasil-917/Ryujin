
#import <Foundation/Foundation.h>

#include "../OS/Platform.hpp"

namespace Ryujin
{

    void MessageBoxOK(const char* header, const char* msg)
    {
        NSAlert *alert = [[NSAlert alloc] init];
        
        NSString *msgStr = [[NSString alloc] initWithUTF8String:msg];
        NSString *headerStr = [[NSString alloc] initWithUTF8String:header];
        [alert addButtonWithTitle:@"OK"];
        //[alert addButtonWithTitle:@"Cancel"];
        [alert setMessageText:headerStr];
        [alert setInformativeText:msgStr];
        [alert setAlertStyle:NSAlertStyleWarning];
        
        [alert runModal];
        
        [msgStr release];
        [headerStr release];
        [alert release];
    }

    int32 BlockingAlert(const char* title, const char* message, const char* button0, const char* button1, const char* button2)
    {
        NSString* titleStr = [NSString stringWithUTF8String:title];
        NSString* msgStr = [NSString stringWithUTF8String:message];
        NSString* button0Str = [NSString stringWithUTF8String:button0];
        NSString* button1Str = button1 ? [NSString stringWithUTF8String:button1] : nil;
        NSString* button2Str = button2 ? [NSString stringWithUTF8String:button2] : nil;
        
        NSAlert *alert = [[[NSAlert alloc] init] autorelease];
        
        [alert setMessageText:titleStr];
        [alert setInformativeText:msgStr];
        [alert setAlertStyle:NSAlertStyleWarning];
        [alert addButtonWithTitle:button0Str];
        if (button1)
        {
            [alert addButtonWithTitle:button1Str];
        }
        if (button2)
        {
            [alert addButtonWithTitle:button2Str];
        }
        
        NSInteger response = [alert runModal];
        
        switch (response)
        {
            case NSAlertFirstButtonReturn:
            default:
                return 0;
                
            case NSAlertSecondButtonReturn:
                return 1;
                
            case NSAlertThirdButtonReturn:
                return 2;
        }
    }

};
