#pragma once


#include "../Common/BufferBase.hpp"

#include "VulkanBase.hpp"

namespace Ryujin
{
	class VulkanBuffer
	{

	};

	class RENDER_API VulkanVertexBuffer : public VertexBufferBase
	{
	private:
		VkBuffer handle;
		VkDeviceMemory memory;

	public:
		VIRTUAL ~VulkanVertexBuffer();
		void Create(const void* data, uint32 elementSize, uint32 count);

		FORCEINLINE VkBuffer GetHandle() const { return handle; }
	};

	class RENDER_API VulkanIndexBuffer : public IndexBufferBase
	{
	private:
		VkBuffer handle;
		VkDeviceMemory memory;

	public:
		VIRTUAL ~VulkanIndexBuffer();
		void Create(const void* data, uint32 elementSize, uint32 count, bool bIs16Bit);

		FORCEINLINE VkBuffer GetHandle() const { return handle; }
	};

	class RENDER_API VulkanConstantBuffer : public ConstantBufferBase
	{
	private:
		VkBuffer handle;
		VkDeviceMemory memory;
		VkDescriptorBufferInfo descriptor;

	public:
		VIRTUAL ~VulkanConstantBuffer();
		void Create(const void* data, uint64 size);
		void Update(void* data, uint64 size);

		FORCEINLINE VkBuffer GetHandle() const { return handle; }
	};

	class RENDER_API VulkanStagingBuffer
	{
	private:
		VkBuffer handle;
		VkDeviceMemory memory;
		uint64 size;

	public:
		~VulkanStagingBuffer();
		void Create(const void* data, uint64 size);
		void Update(void* data, uint64 size);

		FORCEINLINE VkBuffer GetHandle() const { return handle; }
	};

	//class RENDER_API ConstantBuffer
	//{
	//public:
	//	VkBuffer buffer;
	//	VkDeviceMemory memory;
	//	VkDescriptorBufferInfo descriptor;
	//	uint32 allocSize;
	//	void* mapped = nullptr;
	//};
}