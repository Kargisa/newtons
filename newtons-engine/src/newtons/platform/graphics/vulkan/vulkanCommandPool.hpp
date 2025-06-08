#pragma once

#include "vulkan/vulkan.h"
#include "newtons/pch.hpp"
#include "vulkanCommandBuffer.hpp"

namespace nwt
{
    struct VulkanContext;

    class VulkanCommandPool {
        VulkanContext* _context;
        VkCommandPool _pool;

    public:
        VulkanCommandPool()
            : _context(nullptr), _pool(VK_NULL_HANDLE) {
        }

        VulkanCommandPool(VulkanContext* context)
            : _context(context), _pool(VK_NULL_HANDLE) {
        }

        VkCommandPool vkCommandPool() const;

        void initialize(uint32_t queueFamily, VkCommandPoolCreateFlags flags);
        void destroy();

        std::vector<VulkanCommandBuffer> createCommandBuffers(uint32_t count, VkCommandBufferLevel level) const;
    };
} // namespace nwt
