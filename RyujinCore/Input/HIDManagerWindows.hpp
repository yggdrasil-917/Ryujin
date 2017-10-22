#pragma once

#include "HIDManager.hpp"



namespace Ryujin
{

	typedef BOOLEAN(RYUJIN_CALLBACK *HidD_GetAttributes_)(HANDLE device, PHIDD_ATTRIBUTES attrib);
	typedef BOOLEAN(RYUJIN_CALLBACK *HidD_GetSerialNumberString_)(HANDLE device, PVOID buffer, ULONG buffer_len);
	typedef BOOLEAN(RYUJIN_CALLBACK *HidD_GetManufacturerString_)(HANDLE handle, PVOID buffer, ULONG buffer_len);
	typedef BOOLEAN(RYUJIN_CALLBACK *HidD_GetProductString_)(HANDLE handle, PVOID buffer, ULONG buffer_len);
	typedef BOOLEAN(RYUJIN_CALLBACK *HidD_SetFeature_)(HANDLE handle, PVOID data, ULONG length);
	typedef BOOLEAN(RYUJIN_CALLBACK *HidD_GetFeature_)(HANDLE handle, PVOID data, ULONG length);
	typedef BOOLEAN(RYUJIN_CALLBACK *HidD_GetIndexedString_)(HANDLE handle, ULONG string_index, PVOID buffer, ULONG buffer_len);
	typedef BOOLEAN(RYUJIN_CALLBACK *HidD_GetPreparsedData_)(HANDLE handle, PHIDP_PREPARSED_DATA *preparsed_data);
	typedef BOOLEAN(RYUJIN_CALLBACK *HidD_FreePreparsedData_)(PHIDP_PREPARSED_DATA preparsed_data);
	typedef NTSTATUS(RYUJIN_CALLBACK *HidP_GetCaps_)(PHIDP_PREPARSED_DATA preparsed_data, HIDP_CAPS *caps);
	typedef BOOLEAN(RYUJIN_CALLBACK *HidD_SetNumInputBuffers_)(HANDLE handle, ULONG number_buffers);
	typedef BOOLEAN(RYUJIN_CALLBACK *HidD_GetHidGuid_)(LPGUID HidGuid);
	typedef NTSTATUS(RYUJIN_CALLBACK *HidP_GetButtonCaps_)(	HIDP_REPORT_TYPE ReportType,
															PHIDP_BUTTON_CAPS ButtonCaps,
															PUSHORT ButtonCapsLength,
															PHIDP_PREPARSED_DATA PreparsedData);
	typedef NTSTATUS(RYUJIN_CALLBACK *HidP_GetValueCaps_)(	HIDP_REPORT_TYPE     ReportType,
															PHIDP_VALUE_CAPS     ValueCaps,
															PUSHORT              ValueCapsLength,
															PHIDP_PREPARSED_DATA PreparsedData);
	typedef NTSTATUS(RYUJIN_CALLBACK *HidP_GetUsageValue_)(	HIDP_REPORT_TYPE     ReportType,
															USAGE                UsagePage,
															USHORT               LinkCollection,
															USAGE                Usage,
															PULONG               UsageValue,
															PHIDP_PREPARSED_DATA PreparsedData,
															PCHAR                Report,
															ULONG                ReportLength);

	typedef NTSTATUS(RYUJIN_CALLBACK *HidP_GetUsages_)(	HIDP_REPORT_TYPE     ReportType,
														USAGE                UsagePage,
														USHORT               LinkCollection,
														PUSAGE               UsageList,
														PULONG               UsageLength,
														PHIDP_PREPARSED_DATA PreparsedData,
														PCHAR                Report,
														ULONG                ReportLength);
	typedef ULONG(RYUJIN_CALLBACK *HidP_MaxUsageListLength_)(HIDP_REPORT_TYPE     ReportType,
															USAGE                UsagePage,
															PHIDP_PREPARSED_DATA PreparsedData);

	static HidD_GetAttributes_ HidD_GetAttributes;
	static HidD_GetSerialNumberString_ HidD_GetSerialNumberString;
	static HidD_GetManufacturerString_ HidD_GetManufacturerString;
	static HidD_GetProductString_ HidD_GetProductString;
	static HidD_SetFeature_ HidD_SetFeature;
	static HidD_GetFeature_ HidD_GetFeature;
	static HidD_GetIndexedString_ HidD_GetIndexedString;
	static HidD_GetPreparsedData_ HidD_GetPreparsedData;
	static HidD_FreePreparsedData_ HidD_FreePreparsedData;
	static HidP_GetCaps_ HidP_GetCaps;
	static HidD_SetNumInputBuffers_ HidD_SetNumInputBuffers;
	static HidD_GetHidGuid_ HidD_GetHidGuid;
	static HidP_GetButtonCaps_ HidP_GetButtonCaps;
	static HidP_GetValueCaps_ HidP_GetValueCaps;
	static HidP_GetUsageValue_ HidP_GetUsageValue;
	static HidP_GetUsages_ HidP_GetUsages;
	static HidP_MaxUsageListLength_ HidP_MaxUsageListLength;

	class CORE_API HIDManagerWindows : public HIDManager
	{
	private:
		HMODULE hidLibrary = NULL;

		GUID hidGuid;
		HDEVINFO deviceInfoSet;

		SP_DEVINFO_DATA deviceInfoData;
		SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
		HANDLE openDevice;

		uint64 lastCheckDeviceTimestamp;

		bool LoadHidFunctions();
		void CheckEnumeratedDeviceInterface();
		bool CheckDevice(LPCTSTR devicePath, ControllerConfiguration* controllerConfigs, int32 deviceConfigCount, ControllerConfiguration*& outConfig);
		void UpdateDeviceList();

	public:
		HIDManagerWindows(int32 inMaxDevices = DEFAULT_MAX_DEVICE_COUNT);
		~HIDManagerWindows();

		bool Init() OVERRIDE FINAL;
		void Shutdown() OVERRIDE FINAL;
		void UpdateStates(float dt) OVERRIDE FINAL;
		void OnDeviceConnected(void* inDeviceRef) OVERRIDE FINAL;
		void OnDeviceDisconnected(void* inDeviceRef) OVERRIDE FINAL;
	};
}