#pragma once


#include "VulkanCommandEncoder.hpp"


namespace Ryujin
{
	class RENDER_API VulkanCommandBuffer
	{
		friend class VulkanCommandQueue;

	private:
		VkCommandBuffer handle;
		VulkanCommandQueue* queue;

	public:
		void Commit();
		//void Present(class MetalBackBufferImage* image);
		VulkanGfxEncoder* NewGfxEncoderFromDescriptor(const struct RenderPassDescriptor& desc);
		void WaitUntilDone();

		void Release();

		FORCEINLINE VkCommandBuffer GetHandle() const { return handle; }
	};
}