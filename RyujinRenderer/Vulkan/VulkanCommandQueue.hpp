#pragma once

#include "VulkanCommandBuffer.hpp"


namespace Ryujin
{
	class RENDER_API VulkanCommandQueue
	{
	private:
		VkQueue handle;

	public:

		void Create() {}
		VulkanCommandBuffer* NewCommandBuffer() { return nullptr; }
		FORCEINLINE VkQueue GetHandle() const { return handle; }
	};
}