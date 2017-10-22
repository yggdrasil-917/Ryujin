#pragma once


#include "../Containers/DynamicArray.hpp"

#if PLATFORM_MAC
#import <IOKit/hid/IOHIDManager.h>
#elif PLATFORM_LINUX
#include <linux/hid.h>
#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>
#elif PLATFORM_WINDOWS
//#include <xinput.h>
//#pragma comment(lib, "Xinput.lib")


#include <setupapi.h>
#include <winioctl.h>
#pragma comment(lib, "Setupapi.lib")

	/* Copied from inc/ddk/hidclass.h, part of the Windows DDK. */
#define HID_OUT_CTL_CODE(id)  \
		CTL_CODE(FILE_DEVICE_KEYBOARD, (id), METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_HID_GET_FEATURE                   HID_OUT_CTL_CODE(100)

	typedef LONG NTSTATUS;
	/* Since we're not building with the DDK, and the HID header
	files aren't part of the SDK, we have to define all this
	stuff here.  */
typedef struct _HIDD_ATTRIBUTES {
	ULONG Size;
	USHORT VendorID;
	USHORT ProductID;
	USHORT VersionNumber;
} HIDD_ATTRIBUTES, *PHIDD_ATTRIBUTES;

typedef USHORT USAGE;
typedef struct _HIDP_CAPS {
	USAGE    Usage;
	USAGE    UsagePage;
	USHORT   InputReportByteLength;
	USHORT   OutputReportByteLength;
	USHORT   FeatureReportByteLength;
	USHORT   Reserved[17];

	USHORT   NumberLinkCollectionNodes;

	USHORT   NumberInputButtonCaps;
	USHORT   NumberInputValueCaps;
	USHORT   NumberInputDataIndices;

	USHORT   NumberOutputButtonCaps;
	USHORT   NumberOutputValueCaps;
	USHORT   NumberOutputDataIndices;

	USHORT   NumberFeatureButtonCaps;
	USHORT   NumberFeatureValueCaps;
	USHORT   NumberFeatureDataIndices;
} HIDP_CAPS, *PHIDP_CAPS;
typedef void* PHIDP_PREPARSED_DATA;
#ifndef FACILITY_HID_ERROR_CODE
#define FACILITY_HID_ERROR_CODE 0x11
#endif

#define HIDP_ERROR_CODES(SEV, CODE) \
        ((NTSTATUS) (((SEV) << 28) | (FACILITY_HID_ERROR_CODE << 16) | (CODE)))

#define HIDP_STATUS_SUCCESS                  (HIDP_ERROR_CODES(0x0,0))
#define HIDP_STATUS_NULL                     (HIDP_ERROR_CODES(0x8,1))
#define HIDP_STATUS_INVALID_PREPARSED_DATA   (HIDP_ERROR_CODES(0xC,1))
#define HIDP_STATUS_INVALID_REPORT_TYPE      (HIDP_ERROR_CODES(0xC,2))
#define HIDP_STATUS_INVALID_REPORT_LENGTH    (HIDP_ERROR_CODES(0xC,3))
#define HIDP_STATUS_USAGE_NOT_FOUND          (HIDP_ERROR_CODES(0xC,4))
#define HIDP_STATUS_VALUE_OUT_OF_RANGE       (HIDP_ERROR_CODES(0xC,5))
#define HIDP_STATUS_BAD_LOG_PHY_VALUES       (HIDP_ERROR_CODES(0xC,6))
#define HIDP_STATUS_BUFFER_TOO_SMALL         (HIDP_ERROR_CODES(0xC,7))
#define HIDP_STATUS_INTERNAL_ERROR           (HIDP_ERROR_CODES(0xC,8))
#define HIDP_STATUS_I8042_TRANS_UNKNOWN      (HIDP_ERROR_CODES(0xC,9))
#define HIDP_STATUS_INCOMPATIBLE_REPORT_ID   (HIDP_ERROR_CODES(0xC,0xA))
#define HIDP_STATUS_NOT_VALUE_ARRAY          (HIDP_ERROR_CODES(0xC,0xB))
#define HIDP_STATUS_IS_VALUE_ARRAY           (HIDP_ERROR_CODES(0xC,0xC))
#define HIDP_STATUS_DATA_INDEX_NOT_FOUND     (HIDP_ERROR_CODES(0xC,0xD))
#define HIDP_STATUS_DATA_INDEX_OUT_OF_RANGE  (HIDP_ERROR_CODES(0xC,0xE))
#define HIDP_STATUS_BUTTON_NOT_PRESSED       (HIDP_ERROR_CODES(0xC,0xF))
#define HIDP_STATUS_REPORT_DOES_NOT_EXIST    (HIDP_ERROR_CODES(0xC,0x10))
#define HIDP_STATUS_NOT_IMPLEMENTED          (HIDP_ERROR_CODES(0xC,0x20))


typedef enum _HIDP_REPORT_TYPE
{
	HidP_Input,
	HidP_Output,
	HidP_Feature
} HIDP_REPORT_TYPE;

typedef struct _HIDP_BUTTON_CAPS
{
	USAGE    UsagePage;
	UCHAR    ReportID;
	BOOLEAN  IsAlias;

	USHORT   BitField;
	USHORT   LinkCollection;   // A unique internal index pointer

	USAGE    LinkUsage;
	USAGE    LinkUsagePage;

	BOOLEAN  IsRange;
	BOOLEAN  IsStringRange;
	BOOLEAN  IsDesignatorRange;
	BOOLEAN  IsAbsolute;

	ULONG    Reserved[10];
	union {
		struct {
			USAGE    UsageMin, UsageMax;
			USHORT   StringMin, StringMax;
			USHORT   DesignatorMin, DesignatorMax;
			USHORT   DataIndexMin, DataIndexMax;
		} Range;
		struct {
			USAGE    Usage, Reserved1;
			USHORT   StringIndex, Reserved2;
			USHORT   DesignatorIndex, Reserved3;
			USHORT   DataIndex, Reserved4;
		} NotRange;
	};

} HIDP_BUTTON_CAPS, *PHIDP_BUTTON_CAPS;

typedef struct _HIDP_VALUE_CAPS
{
	USAGE    UsagePage;
	UCHAR    ReportID;
	BOOLEAN  IsAlias;

	USHORT   BitField;
	USHORT   LinkCollection;   // A unique internal index pointer

	USAGE    LinkUsage;
	USAGE    LinkUsagePage;

	BOOLEAN  IsRange;
	BOOLEAN  IsStringRange;
	BOOLEAN  IsDesignatorRange;
	BOOLEAN  IsAbsolute;

	BOOLEAN  HasNull;        // Does this channel have a null report   union
	UCHAR    Reserved;
	USHORT   BitSize;        // How many bits are devoted to this value?

	USHORT   ReportCount;    // See Note below.  Usually set to 1.
	USHORT   Reserved2[5];

	ULONG    UnitsExp;
	ULONG    Units;

	LONG     LogicalMin, LogicalMax;
	LONG     PhysicalMin, PhysicalMax;

	union {
		struct {
			USAGE    UsageMin, UsageMax;
			USHORT   StringMin, StringMax;
			USHORT   DesignatorMin, DesignatorMax;
			USHORT   DataIndexMin, DataIndexMax;
		} Range;

		struct {
			USAGE    Usage, Reserved1;
			USHORT   StringIndex, Reserved2;
			USHORT   DesignatorIndex, Reserved3;
			USHORT   DataIndex, Reserved4;
		} NotRange;
	};
} HIDP_VALUE_CAPS, *PHIDP_VALUE_CAPS;

#endif


namespace Ryujin
{
#define DEFAULT_MAX_DEVICE_COUNT 4
    
    enum class ControllerType : uint8
    {
        PS4,
        Xbox360
    };
    
    SMART_ENUM(EControllerButton, uint8,
               Square, X,
               Triangle, A,
               Cross, B,
               Circle, Y,
               LBumper,
               RBumper,
               LTrigger,
               RTrigger,
               LThumbstick,
               RThumbstick,
//               DPadUp,
//               DPadDown,
//               DPadLeft,
//               DPadRight,
               Share,
               Options,
               Touchpad,
               PS);
    
    SMART_ENUM(EControllerAxis, uint8,
               LeftX,
               LeftY,
               RightX,
               RightY,
               LTrigger,
               RTrigger);
    
    SMART_ENUM(EControllerHatswitchValue, int8,
               HatCenter = -1,
               North = 0,
               NorthEast = 1,
               East = 2,
               SouthEast = 3,
               South = 4,
               SouthWest = 5,
               West = 6,
               NorthWest = 7,
               
               HatCount);
    
    struct ControllerButtonConfiguration
    {
        uint32 hidUsage;
        EControllerButton button;
    };
    
    struct ControllerHatConfiguration
    {
        uint32 hidUsage;
        EControllerHatswitchValue hat;
    };
    
    struct ControllerAxisConfiguration
    {
        uint32	hidUsage;
        float	minRange;
        float	maxRange;
        float	deadZone;
        EControllerAxis axis;
    };
    
    struct ButtonState
    {
        EControllerButton button;
        uint32 value; // pressed/released
        uint32 prevValue;
		uint32 hidUsage;
        void* hidElement;
        
        ButtonState()
        : button(EControllerButton::Square),
          value(0),
          prevValue(0),
		  hidUsage(0),
          hidElement(nullptr)
        {
            
        }
    };
    
    struct HatState
    {
        EControllerHatswitchValue value; // pressed/released
        EControllerHatswitchValue prevValue;
        void* hidElement;
        
        HatState()
        : value(EControllerHatswitchValue::HatCenter),
          prevValue(EControllerHatswitchValue::HatCenter),
          hidElement(nullptr)
        {
            
        }
    };
    
    struct AxisState
    {
        EControllerAxis axis;
        long value; // value from the last update
        long minValue;
        long maxValue;
        
        float normValue;
        float minRange;
        float maxRange;
        float deadZone;
		void* hidElement;
		uint32 usage;
        
        AxisState()
        : axis(EControllerAxis::LeftX),
          value(0),
          normValue(0.0f),
          hidElement(nullptr),
		  usage(0)
        {
            
        }
    };
    
    struct ControllerInfo
    {
        int32 controllerId;
        ControllerType type;
        DynamicArray<ButtonState> buttons;
        DynamicArray<HatState> hats;
        DynamicArray<AxisState> axes;
#if PLATFORM_WINDOWS
		PHIDP_PREPARSED_DATA preparsedData = NULL;
		OVERLAPPED readIo;
		CHAR* inputReport = NULL;
		uint32 inputReportByteLength;
#endif
    };
    
    class ControllerConfiguration
    {
    public:
        long vendorId;
        long deviceId;
        long axisCount;
        long buttonCount;
        long hatCount;
        
        ControllerType type;
        ControllerButtonConfiguration* buttons;
        ControllerAxisConfiguration* axes;
        ControllerHatConfiguration* hats;
        
        FORCEINLINE bool IsSameDevice(const long inVendorId, const long inDeviceId,
                                      const long inButtonCount, const long inAxisCount, const long inHatCount)
        {
            return  vendorId == inVendorId && deviceId == inDeviceId &&
                    buttonCount == inButtonCount && axisCount == inAxisCount && hatCount == inHatCount;
        }
    };
    
#if PLATFORM_WINDOWS

typedef USHORT USAGE, *PUSAGE;

#define HID_USAGE_PAGE_BUTTON     0x09

#define HID_USAGE_GENERIC_X       ((USAGE) 0x30 )
#define HID_USAGE_GENERIC_Y       ((USAGE) 0x31 )
#define HID_USAGE_GENERIC_Z       ((USAGE) 0x32 )
#define HID_USAGE_GENERIC_RX      ((USAGE) 0x33 )
#define HID_USAGE_GENERIC_RY      ((USAGE) 0x34 )
#define HID_USAGE_GENERIC_RZ      ((USAGE) 0x35 )

#define HID_USAGE_GENERIC_HATSWITCH ((USAGE) 0x39)

#elif PLATFORM_MAC
#define HID_USAGE_PAGE_BUTTON     0x09
#define HID_USAGE_GENERIC_X       kHIDUsage_GD_X
#define HID_USAGE_GENERIC_Y       kHIDUsage_GD_Y
#define HID_USAGE_GENERIC_Z       kHIDUsage_GD_Z
#define HID_USAGE_GENERIC_RX      kHIDUsage_GD_Rx
#define HID_USAGE_GENERIC_RY      kHIDUsage_GD_Ry
#define HID_USAGE_GENERIC_RZ      kHIDUsage_GD_Rz

#define HID_USAGE_GENERIC_HATSWITCH kHIDUsage_GD_Hatswitch
#elif PLATFORM_LINUX
#define HID_USAGE_PAGE_BUTTON     0x09
#define HID_USAGE_GENERIC_X       0x30
#define HID_USAGE_GENERIC_Y       0x31
#define HID_USAGE_GENERIC_Z       0x32
#define HID_USAGE_GENERIC_RX      0x33
#define HID_USAGE_GENERIC_RY      0x34
#define HID_USAGE_GENERIC_RZ      0x35

#define HID_USAGE_GENERIC_HATSWITCH 0x39
#endif
    
    static ControllerButtonConfiguration buttonsPS4[] =
    {
        { 2, EControllerButton::Cross },
        { 3, EControllerButton::Circle },
        { 1, EControllerButton::Square },
        { 4, EControllerButton::Triangle },
        { 5, EControllerButton::LBumper },
        { 6, EControllerButton::RBumper },
        { 10, EControllerButton::Options },
        { 9, EControllerButton::Share },
        { 11, EControllerButton::LThumbstick },
        { 12, EControllerButton::RThumbstick },
        { 7, EControllerButton::LTrigger },
        { 8, EControllerButton::RTrigger },
        { 14, EControllerButton::Touchpad },
        { 13, EControllerButton::PS },
        { (uint32)-1, EControllerButton::Square }
    };
    
    static ControllerHatConfiguration hatPS4[] =
    {
        { HID_USAGE_GENERIC_HATSWITCH, EControllerHatswitchValue::HatCenter },
        { (uint32)-1, EControllerHatswitchValue::North }
    };
    
    static ControllerAxisConfiguration axesPS4[] =
    {
		{ HID_USAGE_GENERIC_X,  -1.0f, 1.0f, 0.1f, EControllerAxis::LeftX },
		{ HID_USAGE_GENERIC_Y,  1.0f, -1.0f, 0.1f, EControllerAxis::LeftY },
		{ HID_USAGE_GENERIC_Z,  -1.0f, 1.0f, 0.1f, EControllerAxis::RightX },
		{ HID_USAGE_GENERIC_RZ, -1.0f, 1.0f, 0.1f, EControllerAxis::RightY },
		{ HID_USAGE_GENERIC_RX, 0.0f, 1.0f, 0.0f, EControllerAxis::LTrigger },
		{ HID_USAGE_GENERIC_RY, 0.0f, 1.0f, 0.0f, EControllerAxis::RTrigger },
        { (uint32)-1, 0.0f, 0.0f, 0.0f, EControllerAxis::LeftX }
    };

	#define PS4_VENDOR_ID 1356
	#define PS4_PRODUCT_ID 1476
    
    static ControllerConfiguration PS4Controller
    {
		PS4_VENDOR_ID,
		PS4_PRODUCT_ID,
        6,
        14,
        1,
        ControllerType::PS4,
        buttonsPS4,
        axesPS4,
        hatPS4
	};

#define XBOX360_VENDOR_ID 1118
#define XBOX360_PRODUCT_ID 654    
	
	static ControllerButtonConfiguration buttonsXbox360[] =
	{
		{ 1, EControllerButton::A },
		{ 2, EControllerButton::B },
		{ 3, EControllerButton::X },
		{ 4, EControllerButton::Y },
		{ 5, EControllerButton::LBumper },
		{ 6, EControllerButton::RBumper },
		{ 8, EControllerButton::Options },
		{ 7, EControllerButton::Share },
		{ 9, EControllerButton::LThumbstick },
		{ 10, EControllerButton::RThumbstick },
		{ (uint32)-1, EControllerButton::Square }
	};

	static ControllerHatConfiguration hatXbox360[] =
	{
		{ HID_USAGE_GENERIC_HATSWITCH, EControllerHatswitchValue::HatCenter },
		{ (uint32)-1, EControllerHatswitchValue::North }
	};

	static ControllerAxisConfiguration axesXbox360[] =
	{
		{ HID_USAGE_GENERIC_X, -1.0f, 1.0f, 0.1f, EControllerAxis::LeftX },
		{ HID_USAGE_GENERIC_Y,  1.0f, -1.0f, 0.1f, EControllerAxis::LeftY },
		{ HID_USAGE_GENERIC_RX, -1.0f, 1.0f, 0.1f, EControllerAxis::RightX },
		{ HID_USAGE_GENERIC_RY, -1.0f, 1.0f, 0.1f, EControllerAxis::RightY },
		// On 360 Z maps to both triggers but values range differently depending on which trigger is 
		// (L-Trigger: 32K-65K, R-Trigger: 0K-32K)
		{ HID_USAGE_GENERIC_Z, 0.0f, 1.0f, 0.0f, EControllerAxis::LTrigger },
		{ HID_USAGE_GENERIC_Z, 0.0f, 1.0f, 0.0f, EControllerAxis::RTrigger },
		{ (uint32)-1, 0.0f, 0.0f, 0.0f, EControllerAxis::LeftX }
	};

	static ControllerConfiguration Xbox360Controller
	{
		XBOX360_VENDOR_ID,
		XBOX360_PRODUCT_ID,
		6,
		10,
		1,
		ControllerType::Xbox360,
		buttonsXbox360,
		axesXbox360,
		hatXbox360
	};

	static ControllerConfiguration supportedControllerConfigs[] =
	{
		PS4Controller,
		Xbox360Controller
	};

    

#if PLATFORM_MAC
    using DeviceTypePtr = IOHIDDeviceRef;
#elif PLATFORM_WINDOWS
	using DeviceTypePtr = HANDLE;
#else
    using DeviceTypePtr = void*;
#endif
    
    class CORE_API HIDDevice
    {
    public:
        long deviceId;
        long vendorId;
        long axisCount;
        long buttonCount;
        long hatswitchCount;
        DeviceTypePtr hidDevice;
        ControllerInfo controller;
		bool bIsConnected;
		uint64 lastUpdateTimestamp;
		uint32 uniqueId;
        
        HIDDevice();
        ~HIDDevice();
        
        void Init(DeviceTypePtr inDevice, void* otherData = nullptr);
        void Shutdown();
    };
    
    class CORE_API HIDManager
    {
    protected:
        int32 connectedDevices;
        int32 maxDeviceCount;
        DynamicArray<HIDDevice*> devices;
        
    public:
        HIDManager(int32 inMaxDevices = DEFAULT_MAX_DEVICE_COUNT);
        VIRTUAL ~HIDManager();
        
        VIRTUAL bool Init() = 0;
        VIRTUAL void Shutdown() = 0;
        VIRTUAL void UpdateStates(float dt) = 0;
        VIRTUAL void OnDeviceConnected(void* inDeviceRef);
        VIRTUAL void OnDeviceDisconnected(void* inDeviceRef);
        
        FORCEINLINE const DynamicArray<ButtonState>& GetGamepadButtons(int32 gpIndex) const { return devices[gpIndex]->controller.buttons; }
        FORCEINLINE const DynamicArray<HatState>& GetGamepadHatswitches(int32 gpIndex) const { return devices[gpIndex]->controller.hats; }
        FORCEINLINE const DynamicArray<AxisState>& GetGamepadAxes(int32 gpIndex) const { return devices[gpIndex]->controller.axes; }
        FORCEINLINE bool IsDeviceConnected(int32 deviceIndex) const { return devices[deviceIndex] != nullptr; }
        
        int64 GetDeviceVendorId(int32 deviceIndex);
        int64 GetDeviceId(int32 deviceIndex);
        
        FORCEINLINE int32 GetNumDevicesConnected() const { return connectedDevices; }
    };
}
