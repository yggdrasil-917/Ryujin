#pragma once

#include "HIDManager.hpp"



namespace Ryujin
{
    class CORE_API HIDManagerDarwin : public HIDManager
    {
    private:
        IOHIDManagerRef hidManager;
        
    public:
        HIDManagerDarwin(int32 inMaxDevices = DEFAULT_MAX_DEVICE_COUNT);
        ~HIDManagerDarwin();
        
        bool Init() OVERRIDE FINAL;
        void Shutdown() OVERRIDE FINAL;
        void UpdateStates(float dt) OVERRIDE FINAL;
        void OnDeviceConnected(void* inDeviceRef) OVERRIDE FINAL;
        void OnDeviceDisconnected(void* inDeviceRef) OVERRIDE FINAL;
    };
}