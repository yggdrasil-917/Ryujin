#pragma once


#include "VulkanBase.hpp"
//#include "RyujinCore/Logger/Logger.hpp"
#include "RyujinCore/Containers/DynamicArray.hpp"


namespace Ryujin
{
	struct ExampleData
	{
		// List of available frame buffers (same as number of swap chain images)
		DynamicArray<VkFramebuffer> frameBuffers;

		// Color buffer format
		VkFormat colorformat = VK_FORMAT_B8G8R8A8_UNORM;

		// Depth buffer format
		VkFormat depthFormat = VK_FORMAT_D24_UNORM_S8_UINT;

		struct
		{
			VkImage image;
			VkDeviceMemory mem;
			VkImageView view;
		} depthStencil;

		struct {
			VkBuffer buf;
			VkDeviceMemory mem;
			VkPipelineVertexInputStateCreateInfo inputState;
			std::vector<VkVertexInputBindingDescription> bindingDescriptions;
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
		} vertices;

		struct
		{
			int count;
			VkBuffer buf;
			VkDeviceMemory mem;
		} indices;

		struct 
		{
			VkBuffer buffer;
			VkDeviceMemory memory;
			VkDescriptorBufferInfo descriptor;
		}  uniformDataVS;

		struct 
		{
			Matrix44 projectionMatrix;
			Matrix44 modelMatrix;
			Matrix44 viewMatrix;
		} uboVS;

		struct 
		{
			VkPipeline solid;
		} pipelines;

		VkPipelineLayout pipelineLayout;
		VkDescriptorSet descriptorSet;
		VkDescriptorSetLayout descriptorSetLayout;
	};

	struct LayerProperties
	{
		VkLayerProperties properties;
		DynamicArray<VkExtensionProperties> extensions;
	};

	struct SwapChainBuffer
	{
		VkImage image;
		VkImageView view;
	};

	class RENDER_API VulkanDevice
	{
	private:
		int32 width;
		int32 height;
		VkInstance instance;
		VkDevice device;
		VkSwapchainKHR swapChain;
		DynamicArray<SwapChainBuffer> swapChainBuffers;
		VkSurfaceFormatKHR swapChainFormat;
		VkQueue presentQueue;
		DynamicArray<VkCommandBuffer> presentCmdBuffers;
		VkSurfaceKHR surface;
		VkPhysicalDevice physicalDevice;
		VkPhysicalDeviceProperties physicalDeviceProperties;
		VkPhysicalDeviceMemoryProperties memoryProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		uint32 presentQueueIdx;
		uint32 graphicsQueueIdx;
		VkDebugReportCallbackEXT callback;
		VkCommandPool commandPool;
		VkCommandBuffer setupCmdBuffer = VK_NULL_HANDLE;
		DynamicArray<VkCommandBuffer> commandBuffers;

		// Global render pass for frame buffer writes
		VkRenderPass renderPass;

		ExampleData example; // to be removed in the future
		void InitExample();

		VkSemaphore presentComplete;
		VkSemaphore renderComplete;
		class Window* windowRef;

		DynamicArray<VkPhysicalDevice> gpus;
		DynamicArray<LayerProperties> instanceLayerProperties;

		DynamicArray<const char*> deviceLayerNames;
		DynamicArray<const char*> deviceExtensionNames;
		DynamicArray<LayerProperties> deviceLayerProperties;
		DynamicArray<VkExtensionProperties> deviceExtensionProperties;

		VkResult InitGlobalExtensionProperties(LayerProperties& props);
		VkResult InitGlobalLayerProperties();
		VkResult InitInstance(const char* appName);
		bool InitDeviceExtensions(DynamicArray<VkExtensionProperties>& outDeviceProps);

		void CreateSetupCmdBuffer();
		void FlushSetupCmdBuffer();
		bool CreateSwapChain(const uint32 width, const uint32 height);

	public:
		VulkanDevice();
		~VulkanDevice();

		bool CreateDevice();
		bool PostInit(const uint32 width, const uint32 height);
		void Shutdown();

		void RunTest();

		bool CreateSurface(VkSurfaceKHR& surface);
		bool CreateCommandBuffers();
		bool RecordCommandBuffers();

		void Present();
		void OnWindowResize(const uint32 inWidth, const uint32 inHeight);

		//uint32 QueryDeviceCapFeature(const DeviceCapFeature feature);
		void PrintDeviceInfo(const bool bPrintEverything = false);

		FORCEINLINE void CacheWindowReference(Window* inWindow) { windowRef = inWindow; }
	};
}