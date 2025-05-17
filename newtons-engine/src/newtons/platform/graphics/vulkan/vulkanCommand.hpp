#pragma once

#include "vulkan/vulkan.h"
#include "newtons/pch.hpp"

namespace nwt
{
    struct VulkanContext;

    class VulkanCommand {
        VulkanContext* _context;
        VkCommandPool _pool;
        VkCommandBuffer _buffer;
    };
} // namespace nwt
