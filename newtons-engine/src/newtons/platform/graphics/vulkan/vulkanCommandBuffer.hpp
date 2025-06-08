#pragma once

#include "newtons/pch.hpp"
#include "vulkan/vulkan.h"

namespace nwt
{
    struct VulkanCommandPool;
    struct VulkanContext;

    class VulkanCommandBuffer {
        VulkanContext* _context;
        VkCommandBuffer _buffer;

    public:
        VulkanCommandBuffer()
            : _context(nullptr), _buffer(VK_NULL_HANDLE) {
        }

        VulkanCommandBuffer(VulkanContext* context)
            : _context(context), _buffer(VK_NULL_HANDLE) {
        }

        VulkanCommandBuffer(VulkanContext* context, VkCommandBuffer buffer)
            : _context(context), _buffer(buffer) {
        }

        void initialize(const VulkanCommandPool& pool);
        void destroy();

        void reset(VkCommandBufferResetFlags flags) const;

        void begin() const;
        void end() const;

        void setVkCommandBuffer(VkCommandBuffer commandBuffer);

        VkCommandBuffer vkCommandBuffer() const;
        operator VkCommandBuffer() const;

    };
} // namespace nwt
