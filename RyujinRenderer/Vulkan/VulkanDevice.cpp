
#include "VulkanDevice.hpp"
#include "VulkanBuffers.hpp"

#include "RyujinCore/Window/Window.hpp"
#include "RyujinCore/Input/Events.hpp"
#include "RyujinCore/OS/StackTrace.hpp"

namespace Ryujin
{
	static const char* instanceExtensions[] =
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XCB_KHR)
		VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
		VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#endif

#if ENABLE_VALIDATION
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
#endif
	};


	bool CheckExtensionAvailability(const char* extensionToCheck, const DynamicArray<VkExtensionProperties>& extensions)
	{
		for (const auto& extension : extensions)
		{
			//Debugf(Renderer, "Checking extension %s and %s", extension.extensionName, extensionToCheck);
			if (strcmp(extension.extensionName, extensionToCheck) == 0)
			{
				return true;
			}
		}

		return false;
	}

	VulkanDevice* VulkanDevice::Instance()
	{
		static VulkanDevice instance;
		return &instance;
	}

	VkResult VulkanDevice::InitGlobalExtensionProperties(LayerProperties& props)
	{
		VkResult res;
		VkExtensionProperties *instanceExtensions;
		uint32 instanceExtensionCount;
		char *layerName = nullptr;

		layerName = props.properties.layerName;

		do 
		{
			res = vkEnumerateInstanceExtensionProperties(layerName, &instanceExtensionCount, nullptr);
			if (res)
				return res;

			if (instanceExtensionCount == 0)
				return VK_SUCCESS;

			instanceExtensions = RYUJIN_NEW VkExtensionProperties[instanceExtensionCount];
			res = vkEnumerateInstanceExtensionProperties(layerName, &instanceExtensionCount, instanceExtensions);
			props.extensions.MoveTemp(instanceExtensions, instanceExtensionCount);
		} while (res == VK_INCOMPLETE);

		return res;
	}

	VulkanDevice::VulkanDevice()
		: swapChain(nullptr)
	{

	}

	VulkanDevice::~VulkanDevice()
	{

	}

	VkResult VulkanDevice::InitGlobalLayerProperties()
	{
		uint32 instanceLayerCount;
		VkLayerProperties *vkProps = nullptr;
		VkResult res;

		do 
		{
			res = vkEnumerateInstanceLayerProperties(&instanceLayerCount, NULL);
			if (res)
				return res;

			if (instanceLayerCount == 0) 
				return VK_SUCCESS;

			vkProps = (VkLayerProperties *)Memory::Realloc(vkProps, instanceLayerCount * sizeof(VkLayerProperties));

			res = vkEnumerateInstanceLayerProperties(&instanceLayerCount, vkProps);
		} while (res == VK_INCOMPLETE);

		for (uint32 i = 0; i < instanceLayerCount; i++)
		{
			LayerProperties layerProps;
			layerProps.properties = vkProps[i];
			res = InitGlobalExtensionProperties(layerProps);
			if (res)
				return res;
			instanceLayerProperties.Add(layerProps);
		}
		Memory::Free(vkProps);

		return res;
	}

	VkResult VulkanDevice::InitInstance(const char* appName)
	{
		VkApplicationInfo applicationInfo;
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // sType is a member of all structs
        applicationInfo.pNext = nullptr;                               // as is pNext and flag
		applicationInfo.pApplicationName = appName;            // The name of our application
		applicationInfo.pEngineName = appName;                         // The name of the engine
		applicationInfo.engineVersion = 1;                          // The version of the engine
        applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 21);      // The version of Vulkan we're using

		VkInstanceCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &applicationInfo;
#if 0//ENABLE_VALIDATION
        const char *layers[] = { "VK_LAYER_LUNARG_standard_validation" };
		instanceInfo.enabledLayerCount = (uint32)countof(layers);
		instanceInfo.ppEnabledLayerNames = layers;
#else
        instanceInfo.enabledLayerCount = 0;
        instanceInfo.ppEnabledLayerNames = nullptr;
#endif
		instanceInfo.enabledExtensionCount = (uint32)countof(instanceExtensions);
		instanceInfo.ppEnabledExtensionNames = instanceExtensions;

        VkResult result = vkCreateInstance(&instanceInfo, nullptr, &instance);
        return result;
	}

	bool VulkanDevice::InitDeviceExtensions(DynamicArray<VkExtensionProperties>& outDeviceProps)
	{
		uint32 extensionCount = 0;
		if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr) != VK_SUCCESS || extensionCount == 0)
		{
			return false;
		}
		VkExtensionProperties* deviceExtensions = RYUJIN_NEW VkExtensionProperties[extensionCount];

		if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, deviceExtensions) == VK_SUCCESS)
		{
			outDeviceProps.MoveTemp(deviceExtensions, extensionCount);
			return true;
		}

		SAFE_DELETE_ARRAY(deviceExtensions);
		return false;
	}

	void VulkanDevice::CreateSetupCmdBuffer()
	{
		if (setupCmdBuffer != VK_NULL_HANDLE)
		{
			vkFreeCommandBuffers(device, commandPool, 1, &setupCmdBuffer);
			setupCmdBuffer = VK_NULL_HANDLE;
		}

		VkCommandBufferAllocateInfo cmdBufAllocateInfo = VkUtils::Initializers::CommandBufferAllocateInfo(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

		VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &cmdBufAllocateInfo, &setupCmdBuffer));

		VkCommandBufferBeginInfo cmdBufInfo = {};
		cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		VK_CHECK_RESULT(vkBeginCommandBuffer(setupCmdBuffer, &cmdBufInfo));
	}

	void VulkanDevice::FlushSetupCmdBuffer()
	{
		if (setupCmdBuffer == VK_NULL_HANDLE)
			return;

		VK_CHECK_RESULT(vkEndCommandBuffer(setupCmdBuffer));

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &setupCmdBuffer;

		VK_CHECK_RESULT(vkQueueSubmit(presentQueue, 1, &submitInfo, VK_NULL_HANDLE));
		VK_CHECK_RESULT(vkQueueWaitIdle(presentQueue));

		vkFreeCommandBuffers(device, commandPool, 1, &setupCmdBuffer);
		setupCmdBuffer = VK_NULL_HANDLE;
	}

	bool VulkanDevice::CreateSwapChain(const uint32 width, const uint32 height)
	{
		CreateSetupCmdBuffer();

		// Check for swap chain support on running hardware
		VkSurfaceCapabilitiesKHR surfaceCaps;
		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCaps) != VK_SUCCESS)
		{
			return false;
		}

		// Query surface formats
		uint32 surfaceFormatCount = 0;
		if ((vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr) != VK_SUCCESS) || surfaceFormatCount == 0)
		{
			return false;
		}

		VkSurfaceFormatKHR* formats = RYUJIN_NEW VkSurfaceFormatKHR[surfaceFormatCount];
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, formats) != VK_SUCCESS)
		{
			return false;
		}

		// Get present modes
		uint32 presentModeCount = 0;
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr) != VK_SUCCESS || presentModeCount == 0)
		{
			return false;
		}

		VkPresentModeKHR* presentModes = RYUJIN_NEW VkPresentModeKHR[presentModeCount];
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes) != VK_SUCCESS)
		{
			return false;
		}
		VkPresentModeKHR selectedPresentMode = VK_PRESENT_MODE_FIFO_KHR;
		for (uint32 i = 0; i < presentModeCount; ++i)
		{
			// Prefer mailbox mode if it is available as it is the lowest latency non-tearing mode, otherwise
			// immediate is the fastest but doest tear; if not then fallback to fifo which should always be available
			// https://software.intel.com/en-us/articles/api-without-secrets-introduction-to-vulkan-part-2
			if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				selectedPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
				break;
			}
			if (selectedPresentMode != VK_PRESENT_MODE_MAILBOX_KHR && presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				selectedPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
		}

		// minImageCount contains the min number of images for swap chain to work correctly, I add 1 to it and check that it is still safe to do so as 
		// triple buffering can be useful to have
		uint32 imageCount = surfaceCaps.minImageCount + 1;
		if (surfaceCaps.maxImageCount > 0 && imageCount > surfaceCaps.maxImageCount)
		{
			imageCount = surfaceCaps.maxImageCount;
		}

		VkSurfaceFormatKHR selectedSwapChainFormat = { VK_FORMAT_UNDEFINED , VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		// If the list contains only one entry with undefined format
		// it means that there are no preferred surface formats and any can be chosen
		if ((surfaceFormatCount == 1) && (formats[0].format == VK_FORMAT_UNDEFINED)) 
		{
			selectedSwapChainFormat = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		}

		for (uint32 i = 0; i < surfaceFormatCount; ++i)
		{
			if (formats[i].format == VK_FORMAT_R8G8B8A8_UNORM)
			{
				selectedSwapChainFormat = formats[i];
				break;
			}
		}

		if (selectedSwapChainFormat.format == VK_FORMAT_UNDEFINED)
		{
			selectedSwapChainFormat = formats[0];
		}
		swapChainFormat = selectedSwapChainFormat;

		// VK_IMAGE_USAGE_TRANSFER_DST_BIT is required for image clear operation.
		VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		VkExtent2D swapChainExtent = { width, height };
		if (surfaceCaps.currentExtent.width == -1)
		{
			if (swapChainExtent.width < surfaceCaps.minImageExtent.width)
			{
				swapChainExtent.width = surfaceCaps.minImageExtent.width;
			}
			if (swapChainExtent.width > surfaceCaps.maxImageExtent.width)
			{
				swapChainExtent.width = surfaceCaps.maxImageExtent.width;
			}
			if (swapChainExtent.height < surfaceCaps.minImageExtent.height)
			{
				swapChainExtent.height = surfaceCaps.minImageExtent.height;
			}
			if (swapChainExtent.height > surfaceCaps.maxImageExtent.height)
			{
				swapChainExtent.height = surfaceCaps.maxImageExtent.height;
			}
		}

		VkSwapchainKHR oldSwapChain = swapChain;
		VkSwapchainCreateInfoKHR swapChainCreateInfo = 
		{
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,  
			nullptr,                                      
			0,												
			surface,								
			imageCount,										
			selectedSwapChainFormat.format,               
			selectedSwapChainFormat.colorSpace,           
			swapChainExtent,                              
			1,                                            
			usageFlags,										
			VK_SHARING_MODE_EXCLUSIVE,                    
			0,                                            
			nullptr,                                      
			VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,        
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,            
			selectedPresentMode,                         
			VK_TRUE,                                      
			oldSwapChain
		};

		if (vkCreateSwapchainKHR(device, &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS)
		{
			return false;
		}

		if (oldSwapChain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(device, oldSwapChain, nullptr);
		}

		uint32 swapChainCount = 0;
		VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device, swapChain, &swapChainCount, nullptr));

		VkImage* swapChainImages = RYUJIN_NEW VkImage[swapChainCount];
		if (vkGetSwapchainImagesKHR(device, swapChain, &swapChainCount, swapChainImages) != VK_SUCCESS)
		{
			return false;
		}

		swapChainBuffers.Resize(swapChainCount);
		for (uint32 i = 0; i < swapChainCount; i++)
		{
			VkImageViewCreateInfo colorAttachmentView = {};
			colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			colorAttachmentView.pNext = NULL;
			colorAttachmentView.format = swapChainFormat.format;
			colorAttachmentView.components = {
				VK_COMPONENT_SWIZZLE_R,
				VK_COMPONENT_SWIZZLE_G,
				VK_COMPONENT_SWIZZLE_B,
				VK_COMPONENT_SWIZZLE_A
			};
			colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			colorAttachmentView.subresourceRange.baseMipLevel = 0;
			colorAttachmentView.subresourceRange.levelCount = 1;
			colorAttachmentView.subresourceRange.baseArrayLayer = 0;
			colorAttachmentView.subresourceRange.layerCount = 1;
			colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
			colorAttachmentView.flags = 0;

			swapChainBuffers[i].image = swapChainImages[i];

			VkUtils::SetImageLayout(setupCmdBuffer, swapChainBuffers[i].image, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

			colorAttachmentView.image = swapChainBuffers[i].image;

			if (vkCreateImageView(device, &colorAttachmentView, nullptr, &swapChainBuffers[i].view) != VK_SUCCESS)
			{
				return false;
			}
		}

		SAFE_DELETE_ARRAY(swapChainImages);
		SAFE_DELETE_ARRAY(formats);
		SAFE_DELETE_ARRAY(presentModes);

		return true;
	}


	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanReportCallback(VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objectType, uint64 object, size_t location,
		int32 messageCode, const char* layerPrefix, const char* message, void* userData) 
	{
		if (flags & (VK_DEBUG_REPORT_ERROR_BIT_EXT))
		{
			Errorf(Renderer, "VulkanReportCallback -> %s %s", layerPrefix, message);
			PrintStackTrace();

			PushEvent(EngineEvent::ExitOnError);
		}
		return VK_FALSE;
	}

	bool VulkanDevice::CreateDevice()
	{
		VulkanLoader::LoadVulkan();

		uint32 layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, NULL);

        if (InitInstance("Ryujin Engine") != VK_SUCCESS)
        {
            return false;
        }

		uint32 physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

		if (physicalDeviceCount == 0)
		{
			return false;
		}

		if (!CreateSurface(surface))
		{
			return false;
		}

		VkPhysicalDevice* devices = RYUJIN_NEW VkPhysicalDevice[physicalDeviceCount];
		vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, devices);
		gpus.MoveTemp(devices, physicalDeviceCount);

		for (uint32 i = 0; i < physicalDeviceCount; ++i) 
		{
			VkPhysicalDeviceProperties deviceProperties = {};
			vkGetPhysicalDeviceProperties(gpus[i], &deviceProperties);

			Debugf(Renderer, "Found gpu device");
			Debugf(Renderer, "	Device driver: %d", deviceProperties.driverVersion);
			Debugf(Renderer, "	Device name: %s", deviceProperties.deviceName);
			Debugf(Renderer, "	Device type: %d", deviceProperties.deviceType);
			Debugf(Renderer, "	API version: %d.%d.%d", VK_VER_MAJOR(deviceProperties.apiVersion), 
														VK_VER_MINOR(deviceProperties.apiVersion), 
														VK_VER_PATCH(deviceProperties.apiVersion));

			uint32 queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(gpus[i], &queueFamilyCount, NULL);
			VkQueueFamilyProperties *queueFamilyProperties = RYUJIN_NEW VkQueueFamilyProperties[queueFamilyCount];
			vkGetPhysicalDeviceQueueFamilyProperties(gpus[i], &queueFamilyCount, queueFamilyProperties);

			for (uint32 j = 0; j < queueFamilyCount; ++j) 
			{
				VkBool32 supportsPresent;
				vkGetPhysicalDeviceSurfaceSupportKHR(gpus[i], j, surface, &supportsPresent);

				if (supportsPresent && (queueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT))
				{
					physicalDevice = gpus[i];
					physicalDeviceProperties = deviceProperties;
					presentQueueIdx = j;
					graphicsQueueIdx = presentQueueIdx; // TODO: add support for separate graphics queue index when needed
					break;
				}
			}

			SAFE_DELETE_ARRAY(queueFamilyProperties);

			if (physicalDevice)
			{
				break;
			}
		}

		RYUJIN_ASSERT(physicalDevice, "Failed to find a device that supports Vulkan");

		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

		VulkanLoader::LoadVulkanExtensions(instance);
		if (InitGlobalLayerProperties() != VK_SUCCESS)
		{
			return false;
		}

		// Add device extensions
		DynamicArray<VkExtensionProperties> outDeviceProps;
		if (!InitDeviceExtensions(outDeviceProps))
		{
			return false;
		}
		deviceExtensionNames.Add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		for (const auto& deviceExtension : deviceExtensionNames)
		{
			if (!CheckExtensionAvailability(deviceExtension, outDeviceProps))
			{
				return false;
			}
		}

		VkDebugReportCallbackCreateInfoEXT callbackCreateInfo = {};
		callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		callbackCreateInfo.flags =	VK_DEBUG_REPORT_ERROR_BIT_EXT |
									VK_DEBUG_REPORT_WARNING_BIT_EXT |
									VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		callbackCreateInfo.pfnCallback = &VulkanReportCallback;
		callbackCreateInfo.pUserData = NULL;

		VkResult result = vkCreateDebugReportCallbackEXT(instance, &callbackCreateInfo, NULL, &callback);
		if (result != VK_SUCCESS)
		{
			return false;
		}

		VkDeviceQueueCreateInfo queueInfo = {};

		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = presentQueueIdx;
		queueInfo.queueCount = 1;
		float queuePriorities[] = { 1.0f };
		queueInfo.pQueuePriorities = queuePriorities;

		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.pNext = nullptr;
		deviceInfo.queueCreateInfoCount = 1;
		deviceInfo.pQueueCreateInfos = &queueInfo;

#if ENABLE_VALIDATION
		deviceLayerNames.Add("VK_LAYER_LUNARG_standard_validation");
#endif
		deviceInfo.enabledLayerCount = deviceLayerNames.GetSize();
		deviceInfo.ppEnabledLayerNames = deviceInfo.enabledLayerCount ? deviceLayerNames.GetTypedData() : nullptr;

		deviceInfo.enabledExtensionCount = deviceExtensionNames.GetSize();
		deviceInfo.ppEnabledExtensionNames = deviceInfo.enabledExtensionCount ? deviceExtensionNames.GetTypedData() : nullptr;
		deviceInfo.pEnabledFeatures = nullptr;

		VkResult res = vkCreateDevice(gpus[0], &deviceInfo, nullptr, &device);
		if (res != VK_SUCCESS)
		{
			return false;
		}

		return true;
	}

	bool VulkanDevice::PostInit(const uint32 width, const uint32 height)
	{
		this->width = width;
		this->height = height;

		// we can now get the device queue we will be submitting work to (assumption that graphics and presentation queue are the same)
		vkGetDeviceQueue(device, presentQueueIdx, 0, &presentQueue);

		VkSemaphoreCreateInfo semCreateInfo = VkUtils::Initializers::SemaphoreCreateInfo();

		if (vkCreateSemaphore(device, &semCreateInfo, nullptr, &presentComplete) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semCreateInfo, nullptr, &renderComplete) != VK_SUCCESS)
		{
			return false;
		}

		// Create the command pool
		VkCommandPoolCreateInfo cmdPoolCreateInfo =
		{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,     // VkStructureType              sType
			nullptr,                                        // const void*                  pNext
			0,                                              // VkCommandPoolCreateFlags     flags
			presentQueueIdx									// uint32_t                     queueFamilyIndex
		};
		if (vkCreateCommandPool(device, &cmdPoolCreateInfo, nullptr, &commandPool) != VK_SUCCESS)
		{
			return false;
		}

		if (!CreateSwapChain(width, height))
		{
			return false;
		}

		if (!CreateCommandBuffers())
		{
			return false;
		}
		FlushSetupCmdBuffer();

		return true;
	}

	void VulkanDevice::Shutdown()
	{
		if (device != VK_NULL_HANDLE)
		{
			vkDeviceWaitIdle(device);

			if (commandBuffers.GetSize() > 0 && commandBuffers[0] != VK_NULL_HANDLE)
			{
				vkFreeCommandBuffers(device, commandPool, static_cast<uint32>(commandBuffers.GetSize()), &commandBuffers[0]);
				commandBuffers.RemoveAll();
			}

			if (commandPool != VK_NULL_HANDLE)
			{
				vkDestroyCommandPool(device, commandPool, nullptr);
				commandPool = VK_NULL_HANDLE;
			}

			if (presentComplete != VK_NULL_HANDLE)
			{
				vkDestroySemaphore(device, presentComplete, nullptr);
			}
			if (renderComplete != VK_NULL_HANDLE) 
			{
				vkDestroySemaphore(device, renderComplete, nullptr);
			}

			for (auto& buffer : swapChainBuffers)
			{
				vkDestroyImageView(device, buffer.view, nullptr);
			}
			if (swapChain != VK_NULL_HANDLE)
			{
				vkDestroySwapchainKHR(device, swapChain, nullptr);
			}

			vkDestroyDevice(device, nullptr);
		}


		if (surface != VK_NULL_HANDLE)
		{
			vkDestroySurfaceKHR(instance, surface, nullptr);
		}

		if (instance != VK_NULL_HANDLE)
		{
			vkDestroyInstance(instance, nullptr);
		}

		VulkanLoader::UnloadVulkan();
	}

	void VulkanDevice::OnWindowResize(const uint32 inWidth, const uint32 inHeight)
	{
		this->width = inWidth;
		this->height = inHeight;

		CreateSwapChain(inWidth, inHeight);
		FlushSetupCmdBuffer();

		CreateCommandBuffers();
	}

	void VulkanDevice::RunTest()
	{
		uint32 imageIndex;
		VkResult result = vkAcquireNextImageKHR(device, swapChain, NumericLimit<uint64>::Max(), presentComplete, VK_NULL_HANDLE, &imageIndex);
		//switch (result) 
		//{
		//case VK_SUCCESS:
		//case VK_SUBOPTIMAL_KHR:
		//	break;
		//case VK_ERROR_OUT_OF_DATE_KHR:
		//	return;// OnWindowSizeChanged();
		//default:
		//	return;
		//}

		VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;// VK_PIPELINE_STAGE_TRANSFER_BIT;
		VkSubmitInfo submitInfo = 
		{
			VK_STRUCTURE_TYPE_SUBMIT_INFO,                // VkStructureType              sType
			nullptr,                                      // const void                  *pNext
			1,                                            // uint32_t                     waitSemaphoreCount
			&presentComplete,									// const VkSemaphore           *pWaitSemaphores
			&waitDstStageMask,								// const VkPipelineStageFlags  *pWaitDstStageMask;
			1,                                            // uint32_t                     commandBufferCount
			&presentCmdBuffers[imageIndex],					// const VkCommandBuffer       *pCommandBuffers
			1,                                            // uint32_t                     signalSemaphoreCount
			&renderComplete							// const VkSemaphore           *pSignalSemaphores
		};

		if (vkQueueSubmit(presentQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) 
		{
			RYUJIN_ASSERT(0, "Failed to submit Vulkan queue");
		}
		//vkQueueWaitIdle(presentQueue);

		VkPresentInfoKHR presentInfo = 
		{
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,           // VkStructureType              sType
			nullptr,                                      // const void                  *pNext
			1,                                            // uint32_t                     waitSemaphoreCount
			&renderComplete,           // const VkSemaphore           *pWaitSemaphores
			1,                                            // uint32_t                     swapchainCount
			&swapChain,                            // const VkSwapchainKHR        *pSwapchains
			&imageIndex,                                 // const uint32_t              *pImageIndices
			nullptr                                       // VkResult                    *pResults
		};
		result = vkQueuePresentKHR(presentQueue, &presentInfo);

		//switch (result)
		//{
		//case VK_SUCCESS:
		//	break;
		//case VK_ERROR_OUT_OF_DATE_KHR:
		//case VK_SUBOPTIMAL_KHR:
		//	return;// OnWindowSizeChanged();
		//default:
		//	return;
		//}
	}

	void VulkanDevice::Present()
	{

	}

	bool VulkanDevice::CreateSurface(VkSurfaceKHR& surface)
	{
#if PLATFORM_WINDOWS
		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};

		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.hwnd = windowRef->GetWindowHandle();
		surfaceCreateInfo.hinstance = GetModuleHandle(NULL);

		VkResult res = vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);

		return res == VK_SUCCESS;
#elif PLATFORM_LINUX
        VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.connection = windowRef->GetRenderContext();
        surfaceCreateInfo.window = windowRef->GetWindowHandle();

        VkResult res = vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);

        return res == VK_SUCCESS;
#else
		return false;
#endif
	}

	bool VulkanDevice::CreateCommandBuffers()
	{
		uint32 imageCount = 0;
		if ((vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr) != VK_SUCCESS) || (imageCount == 0)) 
		{
			return false;
		}

		presentCmdBuffers.RemoveAll();
		commandBuffers.RemoveAll();

		presentCmdBuffers.AddZeroed(imageCount);
		commandBuffers.AddZeroed(imageCount);

		VkCommandBufferAllocateInfo cmdBufferAllocateInfo = 
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, // VkStructureType              sType
			nullptr,                                        // const void*                  pNext
			commandPool,									// VkCommandPool                commandPool
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,                // VkCommandBufferLevel         level
			imageCount										// uint32_t                     bufferCount
		};
		if (vkAllocateCommandBuffers(device, &cmdBufferAllocateInfo, commandBuffers.GetTypedData()) != VK_SUCCESS ||
			vkAllocateCommandBuffers(device, &cmdBufferAllocateInfo, presentCmdBuffers.GetTypedData()) != VK_SUCCESS)
		{
			return false;
		}

		if (!RecordCommandBuffers())
		{
			return false;
		}

		return true;
	}

	bool VulkanDevice::RecordCommandBuffers()
	{
		uint32 imageCount = static_cast<uint32>(commandBuffers.GetSize());

		VkImage swapChainImages[5]; // There should never be more than this
		if (vkGetSwapchainImagesKHR(device, swapChain, &imageCount, &swapChainImages[0]) != VK_SUCCESS)
		{
			return false;
		}

		VkCommandBufferBeginInfo cmdBufferBeginInfo = VkUtils::Initializers::CommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

		VkClearColorValue clearColor =
		{
			{ 1.0f, 0.8f, 0.4f, 0.0f }
		};

		VkImageSubresourceRange imageSubresourceRange = 
		{
			VK_IMAGE_ASPECT_COLOR_BIT,                    // VkImageAspectFlags                     aspectMask
			0,                                            // uint32_t                               baseMipLevel
			1,                                            // uint32_t                               levelCount
			0,                                            // uint32_t                               baseArrayLayer
			1                                             // uint32_t                               layerCount
		};

		for (uint32 i = 0; i < imageCount; ++i)
		{
			vkBeginCommandBuffer(presentCmdBuffers[i], &cmdBufferBeginInfo);
			VkUtils::SetImageLayout(
				presentCmdBuffers[i], 
				swapChainImages[i], 
				VK_IMAGE_ASPECT_COLOR_BIT, 
				VK_IMAGE_LAYOUT_UNDEFINED, 
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				imageSubresourceRange);

			vkCmdClearColorImage(presentCmdBuffers[i], swapChainImages[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &imageSubresourceRange);
			VkImageMemoryBarrier barrierFromClearToPresent = {
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                        sType
				nullptr,                                    // const void                            *pNext
				VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                          srcAccessMask
				VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                          dstAccessMask
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout                          oldLayout
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,            // VkImageLayout                          newLayout
				presentQueueIdx,             // uint32_t                               srcQueueFamilyIndex
				presentQueueIdx,             // uint32_t                               dstQueueFamilyIndex
				swapChainImages[i],                       // VkImage                                image
				imageSubresourceRange                     // VkImageSubresourceRange                subresourceRange
			};
			vkCmdPipelineBarrier(presentCmdBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrierFromClearToPresent);
			if (vkEndCommandBuffer(presentCmdBuffers[i]) != VK_SUCCESS)
			{
				return false;
			}
		}

		return true;
	}

	void VulkanDevice::InitExample()
	{
		// Setup framebuffers
		VkImageView imageAttachments[2];

		// Depth/Stencil attachment is the same for all frame buffers
		imageAttachments[1] = example.depthStencil.view;

		VkFramebufferCreateInfo frameBufferCreateInfo = {};
		frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferCreateInfo.pNext = NULL;
		frameBufferCreateInfo.renderPass = renderPass;
		frameBufferCreateInfo.attachmentCount = 2;
		frameBufferCreateInfo.pAttachments = imageAttachments;
		frameBufferCreateInfo.width = width;
		frameBufferCreateInfo.height = height;
		frameBufferCreateInfo.layers = 1;

		// Create frame buffers for every swap chain image
		example.frameBuffers.Resize(swapChainBuffers.GetSize());
		for (uint32_t i = 0; i < example.frameBuffers.GetSize(); i++)
		{
			imageAttachments[0] = swapChainBuffers[i].view;
			VkResult err = vkCreateFramebuffer(device, &frameBufferCreateInfo, nullptr, &example.frameBuffers[i]);
			assert(!err);
		}

		// Setup render pass
		VkAttachmentDescription attachments[2] = {};

		// Color attachment
		attachments[0].format = example.colorformat;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// Depth attachment
		attachments[1].format = example.depthFormat;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorReference = {};
		colorReference.attachment = 0;
		colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthReference = {};
		depthReference.attachment = 1;
		depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.flags = 0;
		subpass.inputAttachmentCount = 0;
		subpass.pInputAttachments = NULL;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorReference;
		subpass.pResolveAttachments = NULL;
		subpass.pDepthStencilAttachment = &depthReference;
		subpass.preserveAttachmentCount = 0;
		subpass.pPreserveAttachments = NULL;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.pNext = NULL;
		renderPassInfo.attachmentCount = 2;
		renderPassInfo.pAttachments = attachments;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 0;
		renderPassInfo.pDependencies = NULL;

		VkResult err;

		err = vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass);
		assert(!err);


		VkCommandBufferBeginInfo cmdBufInfo = {};
		cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBufInfo.pNext = NULL;

		VkClearColorValue defaultClearColor =
		{
			{ 1.0f, 0.8f, 0.4f, 0.0f }
		};

		VkClearValue clearValues[2];
		clearValues[0].color = defaultClearColor;
		clearValues[1].depthStencil = { 1.0f, 0 };

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = NULL;
		renderPassBeginInfo.renderPass = renderPass;
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = width;
		renderPassBeginInfo.renderArea.extent.height = height;
		renderPassBeginInfo.clearValueCount = 2;
		renderPassBeginInfo.pClearValues = clearValues;

		uint32 imageCount = static_cast<uint32>(commandBuffers.GetSize());

		for (uint32 i = 0; i < imageCount; ++i)
		{
			// Set target frame buffer
			renderPassBeginInfo.framebuffer = example.frameBuffers[i];

			VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffers[i], &cmdBufInfo));

			// Start the first sub pass specified in our default render pass setup by the base class
			// This will clear the color and depth attachment
			vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

			// Update dynamic viewport state
			VkViewport viewport = {};
			viewport.height = (float)height;
			viewport.width = (float)width;
			viewport.minDepth = (float) 0.0f;
			viewport.maxDepth = (float) 1.0f;
			vkCmdSetViewport(commandBuffers[i], 0, 1, &viewport);

			// Update dynamic scissor state
			VkRect2D scissor = {};
			scissor.extent.width = width;
			scissor.extent.height = height;
			scissor.offset.x = 0;
			scissor.offset.y = 0;
			vkCmdSetScissor(commandBuffers[i], 0, 1, &scissor);

			// Bind descriptor sets describing shader binding points
			vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, example.pipelineLayout, 0, 1, &example.descriptorSet, 0, NULL);

			// Bind the rendering pipeline (including the shaders)
			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, example.pipelines.solid);

			// Bind triangle vertices
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &example.vertices.buf, offsets);

			// Bind triangle indices
			vkCmdBindIndexBuffer(commandBuffers[i], example.indices.buf, 0, VK_INDEX_TYPE_UINT32);

			// Draw indexed triangle
			vkCmdDrawIndexed(commandBuffers[i], example.indices.count, 1, 0, 0, 1);

			vkCmdEndRenderPass(commandBuffers[i]);

			// Add a present memory barrier to the end of the command buffer
			// This will transform the frame buffer color attachment to a
			// new layout for presenting it to the windowing system integration 
			VkImageMemoryBarrier prePresentBarrier = {};
			prePresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			prePresentBarrier.pNext = NULL;
			prePresentBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			prePresentBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			prePresentBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			prePresentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			prePresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			prePresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			prePresentBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
			prePresentBarrier.image = swapChainBuffers[i].image;

			VkImageMemoryBarrier *pMemoryBarrier = &prePresentBarrier;
			vkCmdPipelineBarrier(
				commandBuffers[i],
				VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
				VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &prePresentBarrier);

			VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffers[i]));
		}
	}

	VertexBufferPtr VulkanDevice::NewVertexBuffer(const void* data, uint32 elementSize, uint32 count)
	{
		VertexBufferPtr vb(RYUJIN_NEW VertexBuffer());
		vb->Create(data, elementSize, count);
		return vb;
	}
	IndexBufferPtr VulkanDevice::NewIndexBuffer(const void* data, uint32 elementSize, uint32 count, bool bIs16Bit)
	{
		IndexBufferPtr ib(RYUJIN_NEW IndexBuffer());
		ib->Create(data, elementSize, count, bIs16Bit);
		return ib;
	}
	ConstantBufferPtr VulkanDevice::NewConstantBuffer(const void* data, uint64 bytes)
	{
		ConstantBufferPtr cb(RYUJIN_NEW ConstantBuffer());
		cb->Create(data, bytes);
		return cb;
	}

	VulkanPipelineState* VulkanDevice::NewPipelineState(class VulkanShaderModule* library, const char* vertName, const char* fragName)
	{
		return nullptr;
	}
	VulkanPipelineState* VulkanDevice::NewPipelineState(class VulkanShaderModule* library, const RenderPipelineStateDescriptor& rpd)
	{
		return nullptr;
	}

	class VulkanTexture* VulkanDevice::NewTexture(uint16 width, uint16 height, PixelFormat format, bool mipmapped, TextureUsage usage)
	{
		return nullptr;
	}
	class VulkanTexture* VulkanDevice::NewTexture(const TextureDescriptor& desc)
	{
		return nullptr;
	}

	class VulkanCubemap* VulkanDevice::NewTextureCube(const TextureDescriptor& desc)
	{
		return nullptr;
	}
	class VulkanCubemapArray* VulkanDevice::NewTextureCubeArray(const TextureDescriptor& desc)
	{
		return nullptr;
	}
	class VulkanTextureArray* VulkanDevice::NewTextureArray(const TextureDescriptor& desc)
	{
		return nullptr;
	}
	class VulkanSampler* VulkanDevice::GetOrCreateSampler(SamplerDescriptor& desc)
	{
		return nullptr;
	}
	void VulkanDevice::CopyTextureToBackBuffer(class VulkanTexture* src, class VulkanBackBufferImage* dst)
	{

	}

	void VulkanDevice::PrintDeviceInfo(const bool bPrintEverything /* = false */)
	{
		Debugf(Renderer, "Device name: %s", physicalDeviceProperties.deviceName);
		if (memoryProperties.memoryHeapCount > 0)
		{
			Debugf(Renderer, "Memory: %.2f gigs", (float)memoryProperties.memoryHeaps[0].size / (1024.0f * 1024.0f * 1024.f));
		}
	}

	uint32 VulkanDevice::GetMemoryTypeIndex(uint32 typeBits, VkMemoryPropertyFlags properties)
	{
		// Iterate over all memory types available for the device used in this example
		for (uint32 i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			if ((typeBits & 1) == 1)
			{
				if ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
				{
					return i;
				}
			}
			typeBits >>= 1;
		}

		RYUJIN_ASSERT(0, "Could not find a suitable memory type!");
		return 0;
	}
}
