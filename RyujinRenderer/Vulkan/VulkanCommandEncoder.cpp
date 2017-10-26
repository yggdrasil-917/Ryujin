#include "VulkanCommandEncoder.hpp"
#include "VulkanCommandBuffer.hpp"
#include "VulkanBuffers.hpp"

namespace Ryujin
{
	void VulkanGfxEncoder::PushDebugGroup(const char* name)
	{
		float color[4] = { 0, 1, 0, 1 };
		VkDebugMarkerMarkerInfoEXT markerInfo = {};
		markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
		Memory::MemCpy(markerInfo.color, &color[0], sizeof(float) * 4);
		markerInfo.pMarkerName = name;
		vkCmdDebugMarkerInsertEXT(cmdBuffer->GetHandle(), &markerInfo);
	}

	void VulkanGfxEncoder::PopDebugGroup()
	{
		vkCmdDebugMarkerEndEXT(cmdBuffer->GetHandle());
	}

	void VulkanGfxEncoder::SetPipelineState(const class VulkanPipelineState* pso)
	{

	}

	void VulkanGfxEncoder::SetVertexBufferBytes(const void* data, uint64 size, uint32 slot)
	{

	}

	void VulkanGfxEncoder::SetVertexBuffer(const VulkanVertexBuffer* buffer, uint32 offset, uint32 slot)
	{
		const VkDeviceSize offsets = offset;
		VkBuffer vkBuffer = buffer->GetHandle();
		vkCmdBindVertexBuffers(cmdBuffer->GetHandle(), slot, 1, &vkBuffer, &offsets);
	}

	void VulkanGfxEncoder::SetIndexBuffer(const VulkanIndexBuffer* buffer, uint32 offset, uint32 slot)
	{
		const VkDeviceSize offsets = offset;
		VkBuffer vkBuffer = buffer->GetHandle();
		vkCmdBindIndexBuffer(cmdBuffer->GetHandle(), buffer->GetHandle(), slot, buffer->Is16Bit() ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32);
	}

	void VulkanGfxEncoder::SetVertexConstantBuffer(VulkanConstantBuffer* buffer, uint32 offset, uint32 slot)
	{
		//vkCmdBindDescriptorSets(cmdBuffer->GetHandle(), slot, )
	}

	void VulkanGfxEncoder::SetFragmentConstantBuffer(VulkanConstantBuffer* buffer, uint32 offset, uint32 slot)
	{

	}

	void VulkanGfxEncoder::SetDepthStencilState(CompareFunction compareFunc, bool bDepthWriteEnabled)
	{
	}

	void VulkanGfxEncoder::SetTriangleWinding(FaceWinding winding)
	{
		
	}

	void VulkanGfxEncoder::SetCullingMode(CullMode mode)
	{

	}

	void VulkanGfxEncoder::SetFragmentTexture(class VulkanTexture* texture, uint32 slot)
	{

	}

	void VulkanGfxEncoder::SetFragmentSampler(class VulkanSampler* sampler, uint32 slot)
	{

	}

	void VulkanGfxEncoder::SetViewport(const Rect<float>& rect, float zNear /* = 0.0f */, float zFar /* = 1.0f */)
	{
		VkViewport viewport = {};
		viewport.x = rect.x;
		viewport.y = rect.y;
		viewport.width = rect.width;
		viewport.height = rect.height;
		viewport.minDepth = zNear;
		viewport.maxDepth = zFar;
		vkCmdSetViewport(cmdBuffer->GetHandle(), 0, 1, &viewport);
	}

	void VulkanGfxEncoder::SetScissorRect(const Rect<uint16>& rect)
	{
		VkRect2D vkRect = {};
		vkRect.offset = { rect.x, rect.y };
		vkRect.extent = { rect.width, rect.height };
		vkCmdSetScissor(cmdBuffer->GetHandle(), 0, 1, &vkRect);
	}

	void VulkanGfxEncoder::Draw(uint32 vertexCount, uint32 vertexStart /* = 0 */)
	{
		vkCmdDraw(cmdBuffer->GetHandle(), vertexCount, 1, vertexStart, 0);
	}

	void VulkanGfxEncoder::DrawIndexed(VulkanIndexBuffer* ib)
	{
		vkCmdDrawIndexed(cmdBuffer->GetHandle(), ib->GetIndexCount(), 1, 0, 0, 0);
	}

	void VulkanGfxEncoder::Begin()
	{
		//vkBeginCommandBuffer(cmdBuffer->GetHandle());
	}

	void VulkanGfxEncoder::Finish()
	{
		vkEndCommandBuffer(cmdBuffer->GetHandle());
	}

	void VulkanGfxEncoder::Release()
	{

	}
}