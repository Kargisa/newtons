#pragma once

#include "newtons/pch.hpp"
#include "vulkan/vulkan.h"

namespace nwt
{
    struct VulkanContext;

    class VulkanFence {
        VulkanContext* _context;
        VkFence _fence;

    public:
        VulkanFence()
            : _context(nullptr), _fence(VK_NULL_HANDLE) {
        }

        VulkanFence(VulkanContext* context)
            : _context(context), _fence(VK_NULL_HANDLE) {
        }

        VulkanFence(VulkanContext* context, VkFence fence)
            : _context(context), _fence(fence) {
        }

        void initialize(bool signaled);
        void destroy();

        void wait(uint64_t timeout = UINT64_MAX) const;
        void reset() const;
        VkResult status() const;

        VkFence vkFence() const;
        operator VkFence() const;
    };

} // namespace nwt
