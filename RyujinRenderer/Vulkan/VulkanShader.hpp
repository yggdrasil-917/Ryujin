#pragma once

#include "../Common/ShaderBase.hpp"
#include "VulkanBase.hpp"

namespace Ryujin
{
#define SHADER_EXT ".spv"

	class RENDER_API VulkanShader : public ShaderBase
	{

	};

	class RENDER_API VulkanGraphicsShader : public VulkanShader
	{

	};

	class RENDER_API VulkanComputeShader : public VulkanShader
	{

	};
}