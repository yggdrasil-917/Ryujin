#pragma once

#include "../Common/PipelineStateBase.hpp"
#include "VulkanBase.hpp"


namespace Ryujin
{
	class RENDER_API VulkanPipelineState : public PipelineStateObjectBase
	{
	protected:
		VkPipeline handle;

	public:
		VIRTUAL ~VulkanPipelineState();

		FORCEINLINE VkPipeline GetHandle() const { return handle; }
	};

	class RENDER_API VulkanGfxPipelineState : public VulkanPipelineState
	{

	};

	class RENDER_API VulkanComputePipelineState : public VulkanPipelineState
	{

	};
}