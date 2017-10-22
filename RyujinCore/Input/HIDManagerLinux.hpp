
#pragma once


#include "HIDManager.hpp"

namespace Ryujin
{
    class CORE_API HIDManagerLinux : public HIDManager
    {
    public:
        HIDManagerLinux(int32 inMaxDevices = DEFAULT_MAX_DEVICE_COUNT);
        ~HIDManagerLinux();

        bool Init() OVERRIDE FINAL;
        void Shutdown() OVERRIDE FINAL;
        void UpdateStates(float dt) OVERRIDE FINAL;
        void OnDeviceConnected(void* inDeviceRef) OVERRIDE FINAL;
        void OnDeviceDisconnected(void* inDeviceRef) OVERRIDE FINAL;
    };
}
