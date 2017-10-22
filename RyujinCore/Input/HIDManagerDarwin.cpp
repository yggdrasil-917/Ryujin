

#include "HIDManagerDarwin.hpp"


namespace Ryujin
{
    // function to get a long device property
    // returns FALSE if the property isn't found or can't be converted to a long
    static Boolean IOHIDDevice_GetLongProperty(IOHIDDeviceRef inDeviceRef,     // the HID device reference
                                               CFStringRef inKey,              // the kIOHIDDevice key (as a CFString)
                                               long* outValue)                 // address where to return the output value
    {
        Boolean result = FALSE;
        
        CFTypeRef tCFTypeRef = IOHIDDeviceGetProperty(inDeviceRef, inKey);
        if (tCFTypeRef)
        {
            // if this is a number
            if (CFNumberGetTypeID() == CFGetTypeID(tCFTypeRef))
            {
                // get its value
                result = CFNumberGetValue((CFNumberRef) tCFTypeRef, kCFNumberSInt32Type, outValue);
            }
        }
        
        return result;
    }
    
    // Function taken from Apple sample https://developer.apple.com/library/mac/documentation/DeviceDrivers/Conceptual/HID/new_api_10_5/tn2187.html
    static CFMutableDictionaryRef CreateDeviceMatchingDictionary(UInt32 inUsagePage, UInt32 inUsage)
    {
        // create a dictionary to add usage page/usages to
        CFMutableDictionaryRef result = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                                  0,
                                                                  &kCFTypeDictionaryKeyCallBacks,
                                                                  &kCFTypeDictionaryValueCallBacks);
        if (result)
        {
            if (inUsagePage)
            {
                // Add key for device type to refine the matching dictionary.
                CFNumberRef pageCFNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &inUsagePage);
                if (pageCFNumberRef)
                {
                    CFDictionarySetValue(result, CFSTR(kIOHIDDeviceUsagePageKey), pageCFNumberRef);
                    CFRelease(pageCFNumberRef);
                    
                    // note: the usage is only valid if the usage page is also defined
                    if (inUsage)
                    {
                        CFNumberRef usageCFNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &inUsage);
                        if (usageCFNumberRef)
                        {
                            CFDictionarySetValue(result, CFSTR(kIOHIDDeviceUsageKey), usageCFNumberRef);
                            CFRelease(usageCFNumberRef);
                        }
                        else
                        {
                            Warnf(Input, "CFNumberCreate(usage) failed.");
                        }
                    }
                }
                else
                {
                    Warnf(Input, "CFNumberCreate(usage page) failed.");
                }
            }
        }
        else
        {
            Warnf(Input, "CFDictionaryCreateMutable failed.");
        }
        
        return result;
    }
    
    // USB device added callback function
    static void HandleDeviceMatchingCallback( void *inContext,
                                              IOReturn inResult,
                                              void *inSender,
                                              IOHIDDeviceRef inIOHIDDeviceRef)
    {
        Debugf(Input, "OnDeviceConnected callback");
        
        HIDManagerDarwin* hidManager = static_cast<HIDManagerDarwin*>(inContext);
        if (hidManager)
        {
            hidManager->OnDeviceConnected(inIOHIDDeviceRef);
        }
    }
    
    // USB device removed callback function
    static void HandleDeviceRemovalCallback( void *inContext,
                                             IOReturn inResult,
                                             void *inSender,
                                             IOHIDDeviceRef inIOHIDDeviceRef)
    {
        Debugf(Input, "OnDeviceRemove callback");
        
        HIDManagerDarwin* hidManager = static_cast<HIDManagerDarwin*>(inContext);
        if (hidManager)
        {
            hidManager->OnDeviceDisconnected(inIOHIDDeviceRef);
        }
    }
    
    static void HandleIOHIDInputValueCallback( void *          inContext,      // context from IOHIDManagerRegisterInputValueCallback
                                               IOReturn        inResult,       // completion result for the input value operation
                                               void *          inSender,       // the IOHIDManagerRef
                                               IOHIDValueRef   inIOHIDValueRef // the new element value
                                              )
    {
        IOHIDElementRef element = IOHIDValueGetElement(inIOHIDValueRef);
        Boolean isElement = CFGetTypeID(element) == IOHIDElementGetTypeID();
        if (!isElement)
            return;
        
//        CFStringRef name = IOHIDElementGetName(element);
//        IOHIDElementCookie cookie = IOHIDElementGetCookie(element);
        IOHIDElementType type = IOHIDElementGetType(element);
        int32 usagePage = IOHIDElementGetUsagePage(element);
        int32 usage = IOHIDElementGetUsage(element);

        
        int32 elementValue = IOHIDValueGetIntegerValue(inIOHIDValueRef);
        
        switch (type)
        {
            case kIOHIDElementTypeInput_Misc:
            case kIOHIDElementTypeInput_Axis:
                if( usagePage == kHIDPage_GenericDesktop && IOHIDElementGetReportCount(element) < 2 )
                {
                    if( usage == kHIDUsage_GD_Hatswitch )
                    {
                        Debugf(Input, "HatSwitch at %d, value %d",usage, elementValue);
                    }
                    else
                    {
                        float minValue = IOHIDElementGetPhysicalMin(element);
                        float maxValue = IOHIDElementGetPhysicalMax(element);
                        float axisScale = minValue + (maxValue - minValue) * 0.5f;
                        float axisValue = ((float)(elementValue-axisScale)/axisScale);
                        const float deadzone = 0.2f;
                        
                        if (fabs(axisValue) > deadzone)
                            Debugf(Input, "Axis at %d, %.2f", usage, axisValue);
                    }
                }
                break;
                
            case kIOHIDElementTypeInput_Button:
                Debugf(Input, "Button at %d, state %d",usage,elementValue);
                break;
                
            case kIOHIDElementTypeOutput:
                break;
        }
        
//        Debugf(Input, "Gamepad: %d / %d - [%i] = %d", type, usage, cookie, elementValue);
    }
    
    static void HandleIOHIDReportCallback(void* context, IOReturn result, void* sender,
                                          IOHIDReportType type, uint32 reportID, uint8* report, CFIndex reportLength)
    {
        //Debugf(Input, "HandleIOHIDReportCallback callback");
        
    }
    
    
    
    
    void HIDDevice::Init(DeviceTypePtr inDevice, void* otherData)
    {
        hidDevice = inDevice;
        IOHIDDeviceOpen(hidDevice, kIOHIDOptionsTypeSeizeDevice);
        
        CFArrayRef elements = IOHIDDeviceCopyMatchingElements(inDevice, NULL, 0);
        if(!elements)
        {
            Shutdown();
            return;
        }
        
        long result = 0;
        IOHIDDevice_GetLongProperty(hidDevice, CFSTR(kIOHIDProductIDKey), &result);
        deviceId = result;
        IOHIDDevice_GetLongProperty(hidDevice, CFSTR(kIOHIDVendorIDKey), &result);
        vendorId = result;
        
        CFIndex elementsCount = CFArrayGetCount(elements);
        int32 numberOfAxes = 0;
        int32 numberOfButtons = 0;
        int32 numberOfHatswitches = 0;
        
        for(CFIndex elementIndex = 0; elementIndex < elementsCount; ++elementIndex)
        {
            IOHIDElementRef element = (IOHIDElementRef)CFArrayGetValueAtIndex(elements, elementIndex);
            IOHIDElementType elementType = IOHIDElementGetType( element );
            uint32 usagePage = IOHIDElementGetUsagePage( element );
            uint32 usage = IOHIDElementGetUsage(element);
            
            if(elementType == kIOHIDElementTypeInput_Button && usagePage == kHIDPage_Button)
            {
                ++numberOfButtons;
            }
            else if (usagePage == kHIDPage_GenericDesktop && usage == kHIDUsage_GD_Hatswitch)
            {
                ++numberOfHatswitches;
            }
            else if( ( (elementType == kIOHIDElementTypeInput_Axis) || (elementType == kIOHIDElementTypeInput_Misc) ) && (usagePage == kHIDPage_GenericDesktop) )
            {
                ++numberOfAxes;
            }
        }
        
        axisCount = numberOfAxes;
        buttonCount = numberOfButtons;
        hatswitchCount = numberOfHatswitches;
        Debugf(Input, "HID device detected. Vendor ID is: %d. Product Id is: %d. Number of axes: %d. Number of buttons: %d. Number of hatswitches: %d",
               vendorId, deviceId, axisCount, buttonCount, hatswitchCount);
        
        controller.buttons.RemoveAll();
        controller.buttons.Resize(buttonCount);
        controller.axes.RemoveAll();
        controller.axes.Resize(axisCount);
        controller.hats.RemoveAll();
        controller.hats.Resize(hatswitchCount);
        
        ControllerConfiguration* config = &PS4Controller;
        if (PS4Controller.IsSameDevice(vendorId, deviceId, buttonCount, axisCount, hatswitchCount))
        {
            config = &PS4Controller;
        }
        else if (Xbox360Controller.IsSameDevice(vendorId, deviceId, buttonCount, axisCount, hatswitchCount))
        {
            config = &Xbox360Controller;
        }
        
        // Now configure the device of detected type
        controller.type = config->type;
        for(CFIndex elementIndex = 0; elementIndex < elementsCount; ++elementIndex)
        {
            IOHIDElementRef element = (IOHIDElementRef)CFArrayGetValueAtIndex(elements, elementIndex);
            IOHIDElementType elementType = IOHIDElementGetType(element);
            uint32 usagePage = IOHIDElementGetUsagePage(element);
            uint32 usage = IOHIDElementGetUsage(element);
            
            if(elementType == kIOHIDElementTypeInput_Button && usagePage == kHIDPage_Button)
            {
                
                ButtonState button;
                button.value = 0;
                button.prevValue = 0;
                button.hidElement = (void*)element;
                
                ControllerButtonConfiguration* buttons = config->buttons;
                while(buttons->hidUsage != (uint32)-1)
                {
                    if(buttons->hidUsage == usage)
                    {
                        button.button = buttons->button;
                        controller.buttons.Add(button);
                        break;
                    }
                    ++buttons;
                }
            }
            else if( ( elementType == kIOHIDElementTypeInput_Axis || elementType == kIOHIDElementTypeInput_Misc ) && usagePage == kHIDPage_GenericDesktop )
            {
                IOHIDValueRef value;
                IOHIDDeviceGetValue(hidDevice, element, &value);
                
                if (usage == kHIDUsage_GD_Hatswitch)
                {
                    HatState hat;
                    hat.value = EControllerHatswitchValue::HatCenter;
                    hat.hidElement = (void*)element;
                    
                    ControllerHatConfiguration* hats = config->hats;
                    while(hats->hidUsage != (uint32)-1)
                    {
                        if(hats->hidUsage == usage)
                        {
                            controller.hats.Add(hat);
                            break;
                        }
                        ++hats;
                    }
                }
                else
                {
                    AxisState axis;
                    axis.value = IOHIDValueGetIntegerValue(value);
                    axis.minValue = IOHIDElementGetPhysicalMin(element);
                    axis.maxValue = IOHIDElementGetPhysicalMax(element);
                    axis.hidElement = (void*)element;
                    
                    ControllerAxisConfiguration* axes = config->axes;
                    while(axes->hidUsage != (uint32)-1)
                    {
                        if(axes->hidUsage == usage)
                        {
                            axis.axis = axes->axis;
                            axis.minRange = axes->minRange;
                            axis.maxRange = axes->maxRange;
                            axis.deadZone = axes->deadZone;
                            controller.axes.Add(axis);
                            break;
                        }
                        ++axes;
                    }
                }
            }
        }
    }
    
    void HIDDevice::Shutdown()
    {
        IOHIDDeviceClose(hidDevice, kIOHIDOptionsTypeSeizeDevice);
        hidDevice = nullptr;
    }
    
    
    
    HIDManagerDarwin::HIDManagerDarwin(int32 inMaxDevices)
    : HIDManager(inMaxDevices)
    {
        
    }
    
    
    HIDManagerDarwin::~HIDManagerDarwin()
    {
        CFRelease(hidManager);
    }
    
    bool HIDManagerDarwin::Init()
    {
        hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
        if (!hidManager)
            return false;
        
        // detect any type of game controller
        CFDictionaryRef matchingCFDictRef[3];
        matchingCFDictRef[0] = CreateDeviceMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_Joystick);
        matchingCFDictRef[1] = CreateDeviceMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad);
        matchingCFDictRef[2] = CreateDeviceMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_MultiAxisController);
        CFMutableArrayRef criteria = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
        CFArrayAppendValue(criteria, matchingCFDictRef[0]);
        CFArrayAppendValue(criteria, matchingCFDictRef[1]);
        CFArrayAppendValue(criteria, matchingCFDictRef[2]);
        CFRelease(matchingCFDictRef[0]);
        CFRelease(matchingCFDictRef[1]);
        CFRelease(matchingCFDictRef[2]);
        
        IOHIDManagerSetDeviceMatchingMultiple(hidManager, criteria);
        IOHIDManagerRegisterDeviceMatchingCallback(hidManager, &HandleDeviceMatchingCallback, this);
        IOHIDManagerRegisterDeviceRemovalCallback(hidManager, &HandleDeviceRemovalCallback, this);
        
//        IOHIDManagerRegisterInputValueCallback(hidManager, &HandleIOHIDInputValueCallback, this);
        IOHIDManagerRegisterInputReportCallback(hidManager, &HandleIOHIDReportCallback, this);
        
        
        // Register the HID Manager on our app’s run loop
        IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetMain(), kCFRunLoopDefaultMode);
        
        // Open the HID Manager
        IOReturn ioReturn = IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);
        if (ioReturn)
        {
            Warnf(Input, "Failed to open HID manager");
            return false;
        }
        
        return true;
    }
    
    void HIDManagerDarwin::Shutdown()
    {
        IOHIDManagerUnscheduleFromRunLoop(hidManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
        
//        IOHIDManagerRegisterDeviceMatchingCallback(hidManager, NULL, NULL);
//        IOHIDManagerRegisterDeviceRemovalCallback(hidManager, NULL, NULL);
////        IOHIDManagerRegisterInputValueCallback(hidManager, NULL, NULL);
//        IOHIDManagerRegisterInputReportCallback(hidManager, NULL, NULL);
        
        IOHIDManagerClose(hidManager, kIOHIDOptionsTypeNone); // This also closes hid devices
    }
    
    void HIDManagerDarwin::OnDeviceConnected(void *inDeviceRef)
    {
        if (connectedDevices == maxDeviceCount)
        {
            Debugf(Input, "Max number of devices connected already... ignoring new device");
            return;
        }
        
        HIDManager::OnDeviceConnected(inDeviceRef);
        
        HIDDevice* darwinDevice = RYUJIN_NEW HIDDevice();
        darwinDevice->Init(static_cast<IOHIDDeviceRef>(inDeviceRef));
        darwinDevice->controller.controllerId = connectedDevices;
        
        
        for (int32 i = 0; i < maxDeviceCount; ++i)
        {
            if (devices[i] == nullptr)
            {
                devices[i] = darwinDevice;
                return;
            }
        }
        
        RYUJIN_ASSERT(0, "HIDManagerDarwin::OnDeviceConnected failed to find empty device slot");
    }
    
    void HIDManagerDarwin::OnDeviceDisconnected(void *inDeviceRef)
    {
        HIDManager::OnDeviceDisconnected(inDeviceRef);
        IOHIDDeviceRef dev = static_cast<IOHIDDeviceRef>(inDeviceRef);
        for (int32 i = 0; i < maxDeviceCount; ++i)
        {
            HIDDevice* darwinDev = devices[i];
            if (darwinDev->hidDevice == dev)
            {
                darwinDev->Shutdown();
                delete darwinDev;
                devices[i] = nullptr;
                return;
            }
        }
        
        RYUJIN_ASSERT(0, "HIDManagerDarwin::OnDeviceDisconnected failed to find device to disconnect");
    }
    
    
    void HIDManagerDarwin::UpdateStates(float dt)
    {
        if (!hidManager || connectedDevices == 0)
            return;
        
        for(auto device : devices)
        {
            if (!device || !device->hidDevice)
                continue;
            
            ControllerInfo& controller = device->controller;
            
            // Update all axes
            for (auto& axis : controller.axes)
            {
                IOHIDValueRef value;
                IOReturn result = IOHIDDeviceGetValue(device->hidDevice, (IOHIDElementRef)axis.hidElement, &value);
                if (result == kIOReturnSuccess)
                {
                    long v = IOHIDValueGetIntegerValue(value);
                    if (v != axis.value)
                    {
                        axis.value = v;
                        axis.normValue = (float)(v - axis.minValue) / (axis.maxValue - axis.minValue);
                        if (axis.minRange < 0.0f)
                            axis.normValue = axis.normValue * (axis.maxRange - axis.minRange) - axis.maxRange;
                        else if (axis.maxRange > 1.0f)
                            axis.normValue *= axis.maxRange;
                        if (fabs(axis.normValue) >= axis.deadZone)
                        {
                            Debugf(Input, "Axis %s %.2f", axis.axis.ToString(), axis.normValue);
                        }
                        else
                        {
                            axis.value = axis.minValue;
                            axis.normValue = 0.0f;
                        }
                    }
                }
            }
            
            // Update all buttons
            for (auto& button : controller.buttons)
            {
                IOHIDValueRef value;
                IOReturn result = IOHIDDeviceGetValue(device->hidDevice, (IOHIDElementRef)button.hidElement, &value);
                if(result == kIOReturnSuccess)
                {
                    bool pressed = (IOHIDValueGetIntegerValue(value) > 0);
                    button.prevValue = button.value;
                    button.value = pressed;
                    if(button.prevValue != button.value)
                    {
//                        uint32 usage = IOHIDElementGetUsage( (IOHIDElementRef)button.hidElement );
                        Debugf(Input, "Button %s has changed state", button.button.ToString() );
                    }
                }
            }
            
            // Update all hatswitches
            for (auto& hat : controller.hats)
            {
                IOHIDValueRef value;
                IOReturn result = IOHIDDeviceGetValue(device->hidDevice, (IOHIDElementRef)hat.hidElement, &value);
                if(result == kIOReturnSuccess)
                {
                    long intValue = IOHIDValueGetIntegerValue(value);
                    bool pressed = (intValue >= 0);
                    hat.prevValue = hat.value;
                    
                    if (pressed)
                    {
                        switch (intValue)
                        {
                            case 0:
                                hat.value = EControllerHatswitchValue::North;
                                break;
                                
                            case 1:
                                hat.value = EControllerHatswitchValue::NorthEast;
                                break;
                                
                            case 2:
                                hat.value = EControllerHatswitchValue::East;
                                break;
                                
                            case 3:
                                hat.value = EControllerHatswitchValue::SouthEast;
                                break;
                                
                            case 4:
                                hat.value = EControllerHatswitchValue::South;
                                break;
                                
                            case 5:
                                hat.value = EControllerHatswitchValue::SouthWest;
                                break;
                                
                            case 6:
                                hat.value = EControllerHatswitchValue::West;
                                break;
                                
                            case 7:
                                hat.value = EControllerHatswitchValue::NorthWest;
                                break;
                                
                            default:
                                hat.value = EControllerHatswitchValue::HatCenter;
                                break;
                        }
                    }
                    else
                    {
                        hat.value = EControllerHatswitchValue::HatCenter;
                    }
                    
                    if(hat.prevValue != hat.value)
                    {
                        uint32 usage = IOHIDElementGetUsage( (IOHIDElementRef)hat.hidElement );
                        Debugf(Input, "Hat %d has changed state to %s", usage, hat.value.ToString() );
                    }
                }
            }
        }
    }
}
