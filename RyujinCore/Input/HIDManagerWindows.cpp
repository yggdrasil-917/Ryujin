#include "HIDManagerWindows.hpp"

#include "../Profiler/Profiler.hpp"

namespace Ryujin
{
	struct DeviceCreationDesc
	{
		int32 deviceIndex;
		ControllerConfiguration* config;
		HANDLE hidDeviceHandle;
	};

	void HIDDevice::Init(DeviceTypePtr inDevice, void* otherData)
	{
		DeviceCreationDesc* desc = static_cast<DeviceCreationDesc*>(otherData);
		RYUJIN_ASSERT(desc, "Invalid HID device descriptor received");

		controller.controllerId = desc->deviceIndex;
		controller.type = desc->config->type;
		vendorId = desc->config->vendorId;
		deviceId = desc->config->deviceId;
		hidDevice = desc->hidDeviceHandle;

		controller.buttons.RemoveAll();
		controller.buttons.Resize(desc->config->buttonCount);
		controller.axes.RemoveAll();
		controller.axes.Resize(desc->config->axisCount);
		controller.hats.RemoveAll();
		controller.hats.Resize(desc->config->hatCount);

		// Now we can configure the device
		PHIDP_PREPARSED_DATA preparsedData = NULL;
		HIDP_CAPS caps;
		BOOLEAN result;
		LONG status;

		result = HidD_GetPreparsedData(hidDevice, &preparsedData);
		if (!result)
		{
			Warnf(Input, "Failed to create HID device");
			return;
		}

		status = HidP_GetCaps(preparsedData, &caps);
		if (status < 0)
		{
			HidD_FreePreparsedData(preparsedData);
			return;
		}
		controller.inputReport = RYUJIN_NEW CHAR[caps.InputReportByteLength];
		controller.inputReportByteLength = caps.InputReportByteLength;

		WCHAR ProductName[255];
		ZeroMemory(ProductName, sizeof(ProductName));

		if (HidD_GetProductString(hidDevice, ProductName, 255))
		{
			Debugf(Input, "HID device %ls being initialized", ProductName);
		}

		controller.preparsedData = preparsedData;
		PHIDP_BUTTON_CAPS buttonCaps = (PHIDP_BUTTON_CAPS)malloc(sizeof(HIDP_BUTTON_CAPS) * caps.NumberInputButtonCaps);
		USHORT capsLength = caps.NumberInputButtonCaps;
		if (HidP_GetButtonCaps(HidP_Input, buttonCaps, &capsLength, preparsedData) != HIDP_STATUS_SUCCESS)
		{
			return;
		}

		int32 buttonCount = buttonCaps->Range.UsageMax - buttonCaps->Range.UsageMin + 1;
		for (int32 i = 0; i < buttonCount; ++i)
		{
			ButtonState button;
			button.value = 0;
			button.prevValue = 0;

			ControllerButtonConfiguration* buttons = desc->config->buttons;
			if (buttons)
			{
				while (buttons->hidUsage != (uint32)-1)
				{
					if (buttons->hidUsage == (buttonCaps->Range.UsageMin + i))
					{
						button.button = buttons->button;
						button.hidUsage = buttons->hidUsage;
						controller.buttons.Add(button);
						break;
					}
					++buttons;
				}
			}
		}

		// Handle axes and hatswitches
		if (caps.NumberInputValueCaps > 0)
		{
			PHIDP_VALUE_CAPS axisValues = (PHIDP_VALUE_CAPS)malloc(sizeof(HIDP_VALUE_CAPS) * caps.NumberInputValueCaps);
			capsLength = caps.NumberInputValueCaps;
			if (!axisValues || HidP_GetValueCaps(HidP_Input, axisValues, &capsLength, preparsedData) != HIDP_STATUS_SUCCESS)
			{
				return;
			}
			for (int32 i = 0; i < caps.NumberInputValueCaps; ++i)
			{
				if (axisValues[i].NotRange.Usage == HID_USAGE_GENERIC_HATSWITCH)
				{
					HatState hat;
					hat.value = EControllerHatswitchValue::HatCenter;

					ControllerHatConfiguration* hats = desc->config->hats;
					if (hats)
					{
						while (hats->hidUsage != (uint32)-1)
						{
							if (hats->hidUsage == axisValues[i].NotRange.Usage)
							{
								controller.hats.Add(hat);
								break;
							}
							++hats;
						}
					}
				}
				else
				{
					AxisState axis;
					//axis.minValue = axisValues[i].PhysicalMin;
					//axis.maxValue = axisValues[i].PhysicalMax;
					axis.minValue = axisValues[i].LogicalMin;
					axis.maxValue = axisValues[i].LogicalMax;
					axis.value = axisValues[i].LogicalMin;
					Debugf(Input, "usage: %d, logical [%lu, %lu], physical [%lu, %lu]", axisValues[i].Range.UsageMin,
						axisValues[i].LogicalMin, axisValues[i].LogicalMax,
						axisValues[i].PhysicalMin, axisValues[i].PhysicalMax);

					ControllerAxisConfiguration* axes = desc->config->axes;
					if (axes)
					{
						while (axes->hidUsage != (uint32)-1)
						{
							if (axes->hidUsage == axisValues[i].Range.UsageMin)
							{
								axis.axis = axes->axis;
								axis.minRange = axes->minRange;
								axis.maxRange = axes->maxRange;
								axis.deadZone = axes->deadZone;
								axis.usage = axes->hidUsage;
								controller.axes.Add(axis);
								if (controller.type == ControllerType::Xbox360 && axis.axis == EControllerAxis::LTrigger)
								{
									++axes;
									// duplicate one trigger on 360 controller since they map to same usage
									axis.axis = axes->axis;
									axis.minRange = axes->minRange;
									axis.maxRange = axes->maxRange;
									axis.deadZone = axes->deadZone;
									axis.usage = axes->hidUsage;
									controller.axes.Add(axis);
								}
								break;
							}
							++axes;
						}
					}
				}
			}
			free(axisValues);
		}
		free(buttonCaps);
	}

	void HIDDevice::Shutdown()
	{
		if (hidDevice != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hidDevice);
		}
		hidDevice = nullptr;
		SAFE_DELETE_ARRAY(controller.inputReport);
	}

	bool HIDManagerWindows::LoadHidFunctions()
	{
		hidLibrary = LoadLibrary("HID.DLL");
		if (!hidLibrary)
			return false;

#define RESOLVE(x) x = (x##_)GetProcAddress(hidLibrary, #x); if (!x) return false;
		RESOLVE(HidD_GetAttributes);
		RESOLVE(HidD_GetSerialNumberString);
		RESOLVE(HidD_GetManufacturerString);
		RESOLVE(HidD_GetProductString);
		RESOLVE(HidD_SetFeature);
		RESOLVE(HidD_GetFeature);
		RESOLVE(HidD_GetIndexedString);
		RESOLVE(HidD_GetPreparsedData);
		RESOLVE(HidD_FreePreparsedData);
		RESOLVE(HidP_GetCaps);
		RESOLVE(HidD_SetNumInputBuffers);
		RESOLVE(HidD_GetHidGuid);
		RESOLVE(HidP_GetButtonCaps);
		RESOLVE(HidP_GetValueCaps);
		RESOLVE(HidP_GetUsageValue);
		RESOLVE(HidP_GetUsages);
		RESOLVE(HidP_MaxUsageListLength);
#undef RESOLVE

		return true;
	}

	HIDManagerWindows::HIDManagerWindows(int32 inMaxDevices)
		: HIDManager(inMaxDevices),
		deviceInfoSet(nullptr),
		openDevice(INVALID_HANDLE_VALUE)
	{

	}


	HIDManagerWindows::~HIDManagerWindows()
	{
		if (hidLibrary)
		{
			FreeLibrary(hidLibrary);
			hidLibrary = nullptr;
		}

		if (deviceInfoSet)
		{
			SetupDiDestroyDeviceInfoList(deviceInfoSet);
			deviceInfoSet = nullptr;
		}
	}

	bool HIDManagerWindows::Init()
	{
		if (!LoadHidFunctions())
		{
			return false;
		}

		HidD_GetHidGuid(&hidGuid);

		ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
		deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

		UpdateDeviceList();

		return true;
	}

	void HIDManagerWindows::UpdateDeviceList()
	{
		deviceInfoSet = SetupDiGetClassDevs(&hidGuid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
		if (!deviceInfoSet)
		{
			return;
		}

		DWORD deviceIndex = 0;
		while (SetupDiEnumDeviceInterfaces(deviceInfoSet, NULL, &hidGuid, deviceIndex, &deviceInterfaceData))
		{
			CheckEnumeratedDeviceInterface();
			deviceIndex++;
		}
	}

	void HIDManagerWindows::CheckEnumeratedDeviceInterface()
	{
		DWORD requiredSize;

		SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, NULL, 0, &requiredSize, NULL);

		PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiredSize);
		if (!deviceInterfaceDetailData)
		{
			return;
		}
		deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, deviceInterfaceDetailData, requiredSize, NULL, &deviceInfoData);

		static const int32 deviceConfigCount = (int32)countof(supportedControllerConfigs);
		uint32 crcHash = CRC::MemCrc(deviceInterfaceDetailData->DevicePath, deviceInterfaceDetailData->cbSize);
		bool bDeviceAlreadyConnected = false;
		for (int32 i = 0; i < maxDeviceCount; ++i)
		{
			if (devices[i] && devices[i]->uniqueId == crcHash)
			{
				bDeviceAlreadyConnected = true;
				break;
			}
		}

		ControllerConfiguration* config = nullptr;
		if (!bDeviceAlreadyConnected && CheckDevice(deviceInterfaceDetailData->DevicePath, supportedControllerConfigs, deviceConfigCount, config))
		{
			bool bDeviceConnected = false;
			for (int32 i = 0; i < maxDeviceCount; ++i)
			{
				if (!devices[i])
				{
					DeviceCreationDesc devDesc;
					devDesc.deviceIndex = i;
					devDesc.config = config;
					devDesc.hidDeviceHandle = openDevice;
					OnDeviceConnected(&devDesc);
					bDeviceConnected = true;
					devices[i]->uniqueId = crcHash;
					break;
				}
			}

			if (!bDeviceConnected)
			{
				CloseHandle(openDevice);
			}
		}
		else
		{
			CloseHandle(openDevice);
		}

		openDevice = INVALID_HANDLE_VALUE;
		free(deviceInterfaceDetailData);
	}

	bool HIDManagerWindows::CheckDevice(LPCTSTR devicePath, ControllerConfiguration* controllerConfigs, int32 deviceConfigCount, ControllerConfiguration*& outConfig)
	{
		openDevice = CreateFile(devicePath,
			GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

		if (openDevice == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		HIDD_ATTRIBUTES hidAttributes;
		hidAttributes.Size = sizeof(hidAttributes);

		BOOL Result = HidD_GetAttributes(openDevice, &hidAttributes);
		if (!Result)
		{
			return false;
		}

		for (int32 i = 0; i < deviceConfigCount; ++i)
		{
			ControllerConfiguration* config = &controllerConfigs[i];
			if (hidAttributes.VendorID == config->vendorId && hidAttributes.ProductID == config->deviceId)
			{
				outConfig = config;
				return true;
			}
		}
		return false;
	}

	void HIDManagerWindows::Shutdown()
	{
		for (auto device : devices)
		{
			if (device)
			{
				device->Shutdown();
				SAFE_DELETE(device);
			}
		}
	}

	void HIDManagerWindows::OnDeviceConnected(void* inDeviceRef)
	{
		if (connectedDevices == maxDeviceCount)
		{
			Debugf(Input, "Max number of devices connected already... ignoring new device");
			CloseHandle(openDevice);
			return;
		}

		HIDManager::OnDeviceConnected(inDeviceRef);

		HIDDevice* device = RYUJIN_NEW HIDDevice();
		DeviceCreationDesc* desc = static_cast<DeviceCreationDesc*>(inDeviceRef);
		RYUJIN_ASSERT(desc, "OnDeviceConnected received invalid device");

		device->Init(static_cast<HANDLE>(desc->hidDeviceHandle), inDeviceRef);

		for (int32 i = 0; i < maxDeviceCount; ++i)
		{
			if (devices[i] == nullptr)
			{
				devices[i] = device;
				return;
			}
		}

		RYUJIN_ASSERT(0, "HIDManagerWindows::OnDeviceConnected failed to find empty device slot");
	}

	void HIDManagerWindows::OnDeviceDisconnected(void* inDeviceRef)
	{
		HIDManager::OnDeviceDisconnected(inDeviceRef);
		HANDLE dev = static_cast<HANDLE>(inDeviceRef);
		if (!dev)
		{
			return;
		}

		for (int32 i = 0; i < maxDeviceCount; ++i)
		{
			HIDDevice*& device = devices[i];
			if (device && device->hidDevice == dev)
			{
				device->Shutdown();
				delete device;
				device = nullptr;
				return;
			}
		}

		RYUJIN_ASSERT(0, "HIDManagerWindows::OnDeviceDisconnected failed to find device to disconnect");
	}

	static EControllerHatswitchValue ConvertHatValueToEnum(ControllerType type, long inValue)
	{
		switch (type)
		{
		case ControllerType::Xbox360:
			--inValue; // 360 controller hat values start at 1 instead of 0 like PS4
			break;
		}

		switch (inValue)
		{
		case 0:
			return EControllerHatswitchValue::North;

		case 1:
			return EControllerHatswitchValue::NorthEast;

		case 2:
			return EControllerHatswitchValue::East;

		case 3:
			return EControllerHatswitchValue::SouthEast;

		case 4:
			return EControllerHatswitchValue::South;

		case 5:
			return EControllerHatswitchValue::SouthWest;

		case 6:
			return EControllerHatswitchValue::West;

		case 7:
			return EControllerHatswitchValue::NorthWest;

		default:
			return EControllerHatswitchValue::HatCenter;
		}
	}

	void HIDManagerWindows::UpdateStates(float dt)
	{
		SCOPED_PROFILE_EVENT(HIDManagerUpdateStates, PC_Engine, 255, 0, 0);

		uint64 checkDeviceTimestamp = Time::TimeSinceEpoch();
		if (checkDeviceTimestamp - lastCheckDeviceTimestamp > 1000)
		{
			lastCheckDeviceTimestamp = checkDeviceTimestamp;
			UpdateDeviceList();
		}

		if (connectedDevices == 0)
		{
			return;
		}

		for (auto device : devices)
		{
			if (!device)
			{
				continue;
			}

			ControllerInfo& controller = device->controller;
			DWORD bytesRead = 0;
			HANDLE event = CreateEvent(NULL, FALSE, FALSE, "");
			controller.readIo.hEvent = event;
			controller.readIo.Offset = 0;
			controller.readIo.OffsetHigh = 0;
			BOOL Result = ReadFile(device->hidDevice, controller.inputReport, controller.inputReportByteLength, &bytesRead, &controller.readIo);
			DWORD res = (DWORD)-1;
			if (!Result)
			{
				DWORD err = GetLastError();
				if (ERROR_DEVICE_NOT_CONNECTED == err || ERROR_INVALID_HANDLE == err)
				{
					OnDeviceDisconnected(device->hidDevice);
					continue;
				}
				else if (err == ERROR_IO_PENDING)
				{
					res = WaitForSingleObject(controller.readIo.hEvent, 500);
				}
				else
				{
					Warnf(Input, "ReadFile failed with error %d", GetLastError());
					continue;
				}
			}

			switch (res)
			{
				case WAIT_OBJECT_0:
				{
					Result = GetOverlappedResult(device->hidDevice, &controller.readIo, &bytesRead, TRUE);
					if (!Result)
					{
						DWORD err = GetLastError();
						Debugf(Input, "GetOverlappedResult failed with error %d", err);
					}
					ResetEvent(controller.readIo.hEvent);
					device->lastUpdateTimestamp = Time::TimeSinceEpoch();

					HIDP_CAPS caps;
					NTSTATUS status = HidP_GetCaps(controller.preparsedData, &caps);
					
					// Update axes
					PHIDP_VALUE_CAPS axisValues = (PHIDP_VALUE_CAPS)malloc(sizeof(HIDP_VALUE_CAPS) * caps.NumberInputValueCaps);
					USHORT capsLength = caps.NumberInputValueCaps;
					HidP_GetValueCaps(HidP_Input, axisValues, &capsLength, controller.preparsedData);
					for (int32 i = 0; i < caps.NumberInputValueCaps; ++i)
					{
						ULONG value = 0;
						status = HidP_GetUsageValue(HidP_Input, axisValues[i].UsagePage, axisValues[i].LinkCollection, axisValues[i].Range.UsageMin, &value, controller.preparsedData,
							controller.inputReport, controller.inputReportByteLength);
						if (status == HIDP_STATUS_SUCCESS)
						{
							if (axisValues[i].Range.UsageMin == HID_USAGE_GENERIC_HATSWITCH)
							{
								HatState& hat = controller.hats[0]; // hack assumption that there will only be 1 hatswitch
								hat.prevValue = hat.value;
								hat.value = ConvertHatValueToEnum(controller.type, (long)value);
								if (hat.value != hat.prevValue)
								{
									Debugf(Input, "Hat value changed to %s", hat.value.ToString());
								}
							}
							else
							{
								long v = (long)value;
								for (auto& axis : controller.axes)
								{
									if (axis.usage == axisValues[i].Range.UsageMin)
									{
										if (v == axis.value)
										{
											break;
										}

										if (controller.type == ControllerType::Xbox360 && axis.axis == EControllerAxis::LTrigger)
										{
											// Check for trigger input
											static constexpr uint32 rightTriggerMaxValue = 65535 / 2;
											if (v <= rightTriggerMaxValue)
											{
												// currently processing left trigger when we should be updating right trigger
												// we just continue to next axis since it will be the right trigger with the 
												// current Controller configuration
												continue;
											}
										}
										axis.value = v;
										Debugf(Input, "Axis %s value %ld, [%ld, %ld]", axis.axis.ToString(), v, axis.minValue, axis.maxValue);
										break;
									}
								}
							}
						}
					}
					free(axisValues);

					// Update buttons
					USAGE usageList[32];
					ULONG usageLength = HidP_MaxUsageListLength(HidP_Input, 0, controller.preparsedData);
					status = HidP_GetUsages(HidP_Input, HID_USAGE_PAGE_BUTTON, 0, usageList, &usageLength, controller.preparsedData, controller.inputReport, bytesRead);
					if (status == HIDP_STATUS_SUCCESS)
					{
						// Update button state
						for (auto& button : controller.buttons)
						{
							bool pressed = false;
							for (uint32 i = 0; i < usageLength; ++i)
							{
								if (button.hidUsage == usageList[i])
								{
									pressed = true;
									break;
								}
							}

							button.prevValue = button.value;
							button.value = pressed;
							if (button.prevValue != button.value)
							{
								Debugf(Input, "Button %s has changed state", button.button.ToString());
							}
						}
					}
					break;
				}
				case WAIT_TIMEOUT:
				{
					CancelIo(device->hidDevice);
					break;
				}
			}
		}

		// Check device last update timestamps to see if maybe a device has been disconnected
		//for (auto device : devices)
		//{
		//	if (device && Time::TimeSinceEpoch() - device->lastUpdateTimestamp >= 2000)
		//	{
		//		// more than 2 seconds since the last update, either device was disconnected or there is another problem with it
		//		OnDeviceDisconnected(device->hidDevice);
		//	}
		//}
	}
}

