
#include "HIDManager.hpp"



namespace Ryujin
{
    HIDDevice::HIDDevice()
    : deviceId(0),
      vendorId(0),
	  bIsConnected(false),
	  lastUpdateTimestamp(0),
	  uniqueId(0)
    {
        
    }
    
    HIDDevice::~HIDDevice()
    {
        vendorId = 0;
        deviceId = 0;
		lastUpdateTimestamp = 0;
		uniqueId = 0;
		RYUJIN_ASSERT(!bIsConnected, "Destroyed HID device without properly disconnecting");
    }
    
    HIDManager::HIDManager(int32 inMaxDevices)
    : connectedDevices(0),
      maxDeviceCount(inMaxDevices)
    {
        devices.AddZeroed(inMaxDevices);
    }
    
    
    HIDManager::~HIDManager()
    {
        for (auto& it : devices)
        {
            if (it)
            {
                delete it;
            }
        }
    }
    
    void HIDManager::OnDeviceConnected(void* inDeviceRef)
    {
        ++connectedDevices;
    }
    
    void HIDManager::OnDeviceDisconnected(void* inDeviceRef)
    {
        --connectedDevices;
    }
    
    int64 HIDManager::GetDeviceId(int32 deviceIndex)
    {
        HIDDevice* device = devices[deviceIndex];
        if (device)
        {
            return device->deviceId;
        }
        
        return 0;
    }
    
    int64 HIDManager::GetDeviceVendorId(int32 deviceIndex)
    {
        HIDDevice* device = devices[deviceIndex];
        if (device)
        {
            return device->vendorId;
        }
        
        return 0;
    }
}
