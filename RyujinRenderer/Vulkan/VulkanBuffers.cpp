#include "VulkanBuffers.hpp"
#include "VulkanDevice.hpp"

namespace Ryujin
{
	// from https://github.com/SaschaWillems/Vulkan/blob/master/triangle/triangle.cpp
	// This function is used to request a device memory type that supports all the property flags we request (e.g. device local, host visibile)
	// Upon success it will return the index of the memory type that fits our requestes memory properties
	// This is necessary as implementations can offer an arbitrary number of memory types with different
	// memory properties. 
	// You can check http://vulkan.gpuinfo.org/ for details on different memory configurations
	static uint32 GetMemoryTypeIndex(uint32 typeBits, VkMemoryPropertyFlags properties, VkPhysicalDeviceMemoryProperties deviceMemoryProperties)
	{
		// Iterate over all memory types available for the device used in this example
		for (uint32 i = 0; i < deviceMemoryProperties.memoryTypeCount; i++)
		{
			if ((typeBits & 1) == 1)
			{
				if ((deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
				{
					return i;
				}
			}
			typeBits >>= 1;
		}

		RYUJIN_ASSERT(0, "Could not find a suitable memory type!");
		return 0;
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		vkDestroyBuffer(VulkanDevice::Instance()->GetDevice(), handle, nullptr);
		vkFreeMemory(VulkanDevice::Instance()->GetDevice(), memory, nullptr);
	}

	void VulkanVertexBuffer::Create(const void* data, uint32 elementSize, uint32 count)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = elementSize * count;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK_RESULT(vkCreateBuffer(VulkanDevice::Instance()->GetDevice(), &bufferInfo, nullptr, &handle));

		VkMemoryRequirements vertexBufferMemoryRequirements = {};
		vkGetBufferMemoryRequirements(VulkanDevice::Instance()->GetDevice(), handle, &vertexBufferMemoryRequirements);

		VkMemoryAllocateInfo bufferAllocateInfo = {};
		bufferAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		bufferAllocateInfo.allocationSize = vertexBufferMemoryRequirements.size;

		uint32_t vertexMemoryTypeBits = vertexBufferMemoryRequirements.memoryTypeBits;
		VkMemoryPropertyFlags vertexDesiredMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT; 
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(VulkanDevice::Instance()->GetPhysicalDevice(), &memProperties);
		for (uint32_t i = 0; i < 32; ++i)
		{
			VkMemoryType memoryType = memProperties.memoryTypes[i];
			if (vertexMemoryTypeBits & 1) 
			{
				if ((memoryType.propertyFlags & vertexDesiredMemoryFlags) == vertexDesiredMemoryFlags) 
				{
					bufferAllocateInfo.memoryTypeIndex = i;
					break;
				}
			}
			vertexMemoryTypeBits = vertexMemoryTypeBits >> 1;
		}

		VK_CHECK_RESULT(vkAllocateMemory(VulkanDevice::Instance()->GetDevice(), &bufferAllocateInfo, NULL, &memory));

		if (data)
		{
			void* mapped = nullptr;
			VK_CHECK_RESULT(vkMapMemory(VulkanDevice::Instance()->GetDevice(), memory, 0, VK_WHOLE_SIZE, 0, &mapped));
			Memory::MemCpy(mapped, data, elementSize * count);
			vkUnmapMemory(VulkanDevice::Instance()->GetDevice(), memory);
			VK_CHECK_RESULT(vkBindBufferMemory(VulkanDevice::Instance()->GetDevice(), handle, memory, 0));
		}
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		vkDestroyBuffer(VulkanDevice::Instance()->GetDevice(), handle, nullptr);
		vkFreeMemory(VulkanDevice::Instance()->GetDevice(), memory, nullptr);
	}

	void VulkanIndexBuffer::Create(const void* data, uint32 elementSize, uint32 count, bool bIs16Bit)
	{
		// currently not using staging buffers so it is slow
		VkDevice device = VulkanDevice::Instance()->GetDevice();
		VkBufferCreateInfo indexbufferInfo = {};
		indexbufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexbufferInfo.size = count * elementSize;
		indexbufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

		VkMemoryRequirements indexBufferMemoryRequirements = {};
		vkGetBufferMemoryRequirements(device, handle, &indexBufferMemoryRequirements);

		VkMemoryAllocateInfo bufferAllocateInfo = {};
		bufferAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		bufferAllocateInfo.allocationSize = indexBufferMemoryRequirements.size;

		// Copy index data to a buffer visible to the host
		VK_CHECK_RESULT(vkCreateBuffer(device, &indexbufferInfo, nullptr, &handle));
		vkGetBufferMemoryRequirements(device, handle, &indexBufferMemoryRequirements);
		bufferAllocateInfo.allocationSize = indexBufferMemoryRequirements.size;
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(VulkanDevice::Instance()->GetPhysicalDevice(), &memProperties);
		bufferAllocateInfo.memoryTypeIndex = GetMemoryTypeIndex(indexBufferMemoryRequirements.memoryTypeBits, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			memProperties);
		VK_CHECK_RESULT(vkAllocateMemory(device, &bufferAllocateInfo, nullptr, &memory));
		void* mapped = nullptr;
		VK_CHECK_RESULT(vkMapMemory(device, memory, 0, count * elementSize, 0, &mapped));
		Memory::MemCpy(mapped, data, elementSize * count);
		vkUnmapMemory(device, memory);
		VK_CHECK_RESULT(vkBindBufferMemory(device, handle, memory, 0));
	}

	VulkanConstantBuffer::~VulkanConstantBuffer()
	{
		vkDestroyBuffer(VulkanDevice::Instance()->GetDevice(), handle, nullptr);
		vkFreeMemory(VulkanDevice::Instance()->GetDevice(), memory, nullptr);
	}

	void VulkanConstantBuffer::Create(const void* data, uint64 size)
	{
		VkDevice device = VulkanDevice::Instance()->GetDevice();
		// Prepare and initialize a uniform buffer block containing shader uniforms
		// Single uniforms like in OpenGL are no longer present in Vulkan. All Shader uniforms are passed via uniform buffer blocks
		VkMemoryRequirements memReqs;

		// Vertex shader uniform buffer block
		VkBufferCreateInfo bufferInfo = {};
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.allocationSize = 0;
		allocInfo.memoryTypeIndex = 0;

		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		// This buffer will be used as a uniform buffer
		bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		// Create a new buffer
		VK_CHECK_RESULT(vkCreateBuffer(device, &bufferInfo, nullptr, &handle));
		// Get memory requirements including size, alignment and memory type 
		vkGetBufferMemoryRequirements(device, handle, &memReqs);
		allocInfo.allocationSize = memReqs.size;
		// Get the memory type index that supports host visibility memory access
		// Most implementations offer multiple memory types and selecting the correct one to allocate memory from is crucial
		// We also want the buffer to be host coherent so we don't have to flush (or sync after every update.
		// Note: This may affect performance so you might not want to do this in a real world application that updates buffers on a regular base
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(VulkanDevice::Instance()->GetPhysicalDevice(), &memProperties);
		allocInfo.memoryTypeIndex = GetMemoryTypeIndex(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, memProperties);
		// Allocate memory for the uniform buffer
		VK_CHECK_RESULT(vkAllocateMemory(device, &allocInfo, nullptr, &memory));
		// Bind memory to buffer
		VK_CHECK_RESULT(vkBindBufferMemory(device, handle, memory, 0));

		// Store information in the uniform's descriptor that is used by the descriptor set
		descriptor.buffer = handle;
		descriptor.offset = 0;
		descriptor.range = size;
	}

	void VulkanConstantBuffer::Update(void* data, uint64 size)
	{
		VkDevice device = VulkanDevice::Instance()->GetDevice();
		void* mapped = nullptr;
		VK_CHECK_RESULT(vkMapMemory(device, memory, 0, size, 0, &mapped));
		Memory::MemCpy(mapped, data, size);
		// Unmap after data has been copied
		// Note: Since we requested a host coherent memory type for the uniform buffer, the write is instantly visible to the GPU
		vkUnmapMemory(device, memory);
	}

	VulkanStagingBuffer::~VulkanStagingBuffer()
	{
		VkDevice device = VulkanDevice::Instance()->GetDevice();
		vkFreeMemory(device, memory, nullptr);
		vkDestroyBuffer(device, handle, nullptr);
	}

	void VulkanStagingBuffer::Create(const void* data, uint64 size)
	{
		VkDevice device = VulkanDevice::Instance()->GetDevice();
		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = size;
		// This buffer is used as a transfer source for the buffer copy
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK_RESULT(vkCreateBuffer(device, &bufferCreateInfo, nullptr, &handle));

		// Get memory requirements for the staging buffer (alignment, memory type bits)
		VkMemoryRequirements memReqs = {};
		vkGetBufferMemoryRequirements(device, handle, &memReqs);

		VkMemoryAllocateInfo memAllocInfo = {};
		memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memAllocInfo.pNext = NULL;
		memAllocInfo.allocationSize = 0;
		memAllocInfo.memoryTypeIndex = 0;
		memAllocInfo.allocationSize = memReqs.size;
		// Get memory type index for a host visible buffer
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(VulkanDevice::Instance()->GetPhysicalDevice(), &memProperties);
		memAllocInfo.memoryTypeIndex = GetMemoryTypeIndex(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, memProperties);

		VK_CHECK_RESULT(vkAllocateMemory(device, &memAllocInfo, nullptr, &memory));
		VK_CHECK_RESULT(vkBindBufferMemory(device, handle, memory, 0));

		if (data)
		{
			void* mapped = nullptr;
			VK_CHECK_RESULT(vkMapMemory(device, memory, 0, memReqs.size, 0, &mapped));
			Memory::MemCpy(mapped, data, size);
			vkUnmapMemory(device, memory);
		}
		this->size = size;
	}

	void VulkanStagingBuffer::Update(void* data, uint64 size)
	{
		RYUJIN_ASSERT(this->size >= size, *String::Printf("Staging buffer is %d bytes, tried to copy %d bytes", this->size, size));
		VkDevice device = VulkanDevice::Instance()->GetDevice();
		void* mapped = nullptr;
		VK_CHECK_RESULT(vkMapMemory(device, memory, 0, size, 0, &mapped));
		Memory::MemCpy(mapped, data, size);
		vkUnmapMemory(device, memory);
	}
}