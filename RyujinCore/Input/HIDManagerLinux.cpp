#include "HIDManagerLinux.hpp"


namespace Ryujin
{
    HIDManagerLinux::HIDManagerLinux(int32 inMaxDevices)
        : HIDManager(inMaxDevices)
    {

    }

    HIDManagerLinux::~HIDManagerLinux()
    {

    }

    bool HIDManagerLinux::Init()
    {
        return true;
    }

    void HIDManagerLinux::Shutdown()
    {

    }

    void HIDManagerLinux::UpdateStates(float dt)
    {

    }

    void HIDManagerLinux::OnDeviceConnected(void* inDeviceRef)
    {
        HIDManager::OnDeviceConnected(inDeviceRef);
    }

    void HIDManagerLinux::OnDeviceDisconnected(void *inDeviceRef)
    {
        HIDManager::OnDeviceDisconnected(inDeviceRef);
    }
}
