#pragma once

#include "../Renderer/RendererBase.hpp"
#include "../Common/ResourceDescriptors.hpp"

#include "VulkanBase.hpp"


namespace Ryujin
{
	class RENDER_API VulkanCommandEncoder
	{
		friend class VulkanCommandBuffer;

	protected:
		class VulkanCommandBuffer* cmdBuffer;

	public:
		VIRTUAL ~VulkanCommandEncoder() {}
	};

	class RENDER_API VulkanGfxEncoder : public VulkanCommandEncoder
	{
	private:

	public:

		void PushDebugGroup(const char* name);
		void PopDebugGroup();
		void SetPipelineState(const class VulkanPipelineState* pso);
		void SetVertexBufferBytes(const void* data, uint64 size, uint32 slot);
		void SetVertexBuffer(const class VulkanVertexBuffer* buffer, uint32 offset, uint32 slot);
		void SetIndexBuffer(const class VulkanIndexBuffer* buffer, uint32 offset, uint32 slot);
		void SetVertexConstantBuffer(class VulkanConstantBuffer* buffer, uint32 offset, uint32 slot);
		void SetFragmentConstantBuffer(class VulkanConstantBuffer* buffer, uint32 offset, uint32 slot);
		void SetDepthStencilState(CompareFunction compareFunc, bool bDepthWriteEnabled);
		void SetTriangleWinding(FaceWinding winding);
		void SetCullingMode(CullMode mode);
		void SetFragmentTexture(class VulkanTexture* texture, uint32 slot);
		void SetFragmentSampler(class VulkanSampler* sampler, uint32 slot);
		void SetViewport(const Rect<float>& rect, float zNear = 0.0f, float zFar = 1.0f);
		void SetScissorRect(const Rect<uint16>& rect);
		void Draw(uint32 vertexCount, uint32 vertexStart = 0);
		void DrawIndexed(VulkanIndexBuffer* ib);
		void Begin();
		void Finish();

		void Release();

		FORCEINLINE class VulkanCommandBuffer* GetHandle() const { return cmdBuffer; }
	};

	class RENDER_API VulkanComputeEncoder : public VulkanCommandEncoder
	{

	};
}