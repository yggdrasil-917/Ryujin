#pragma once


#include "RendererBase.hpp"
#if PLATFORM_MAC

#include "../Metal/MetalRendererCommon.hpp"

#elif PLATFORM_WINDOWS

#if USE_VULKAN
#include "../Vulkan/VulkanRendererCommon.hpp"
#else
#error "Renderer not implemented for platform"
#endif

#else // Linux

#include "../Vulkan/VulkanRendererCommon.hpp"

#endif

#include "Visibility.hpp"




namespace Ryujin
{
#include "ShaderCommon.hpp"
}


