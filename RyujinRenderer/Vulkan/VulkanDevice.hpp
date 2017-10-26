#pragma once


#include "VulkanBase.hpp"
//#include "RyujinCore/Logger/Logger.hpp"
#include "RyujinCore/Containers/DynamicArray.hpp"
#include "../Renderer/Buffer.hpp"

#include "../Common/ResourceDescriptors.hpp"

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

		class VulkanCommandQueue* commandQueue;
		class VulkanSwapChain* swapChain2;

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

		static VulkanDevice* Instance();

		bool CreateDevice();
		bool PostInit(const uint32 width, const uint32 height);
		void Shutdown();

		void RunTest();

		bool CreateSurface(VkSurfaceKHR& surface);
		bool CreateCommandBuffers();
		bool RecordCommandBuffers();

		VertexBufferPtr NewVertexBuffer(const void* data, uint32 elementSize, uint32 count);
		IndexBufferPtr NewIndexBuffer(const void* data, uint32 elementSize, uint32 count, bool bIs16Bit);
		ConstantBufferPtr NewConstantBuffer(const void* data, uint64 bytes);

		class VulkanPipelineState* NewPipelineState(class VulkanShaderModule* library, const char* vertName, const char* fragName);
		class VulkanPipelineState* NewPipelineState(class VulkanShaderModule* library, const RenderPipelineStateDescriptor& rpd);

		class VulkanTexture* NewTexture(uint16 width, uint16 height, PixelFormat format, bool mipmapped, TextureUsage usage);
		class VulkanTexture* NewTexture(const TextureDescriptor& desc);
		class VulkanCubemap* NewTextureCube(const TextureDescriptor& desc);
		class VulkanCubemapArray* NewTextureCubeArray(const TextureDescriptor& desc);
		class VulkanTextureArray* NewTextureArray(const TextureDescriptor& desc);
		class VulkanSampler* GetOrCreateSampler(SamplerDescriptor& desc);
		void CopyTextureToBackBuffer(class VulkanTexture* src, class VulkanBackBufferImage* dst);

		void Present();
		void OnWindowResize(const uint32 inWidth, const uint32 inHeight);

		//uint32 QueryDeviceCapFeature(const DeviceCapFeature feature);
		void PrintDeviceInfo(const bool bPrintEverything = false);

		FORCEINLINE void CacheWindowReference(Window* inWindow) { windowRef = inWindow; }
		FORCEINLINE VkDevice GetDevice() const { return device; }
		FORCEINLINE VkPhysicalDevice GetPhysicalDevice() const { return physicalDevice; }
		FORCEINLINE class VulkanCommandQueue* GetImmediateCmdQueue() { return commandQueue; }
		FORCEINLINE class VulkanSwapChain* GetSwapChain() const { return swapChain2; }
	};
}