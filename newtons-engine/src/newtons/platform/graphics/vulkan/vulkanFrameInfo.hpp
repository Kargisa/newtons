#pragma once

#include "vulkanFramebuffer.hpp"
#include "vulkanCommandPool.hpp"
#include "vulkanCommandBuffer.hpp"
#include "vulkanFence.hpp"
#include "vulkanSemaphore.hpp"

namespace nwt
{
    // *********************************************
    // ************** Per Frame Info ***************
    // *********************************************

    struct VulkanFrameInfo
    {
        VulkanCommandPool commandPool;
        VulkanCommandBuffer commandBuffer;
        VulkanFence fence;
        VulkanSemaphore imageAvailabeSemaphore;
    };
} // namespace nwt
