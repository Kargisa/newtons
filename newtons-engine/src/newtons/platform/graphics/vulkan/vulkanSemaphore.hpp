#pragma once

#include "vulkan/vulkan.h"
#include "newtons/pch.hpp"

namespace nwt
{
    struct VulkanContext;

    class VulkanSemaphore {
        VulkanContext* _context;
        VkSemaphore _semaphore;

    public:
        VulkanSemaphore()
            : _context(nullptr), _semaphore(VK_NULL_HANDLE) {

        }

        VulkanSemaphore(VulkanContext* context)
            : _context(context), _semaphore(VK_NULL_HANDLE) {
        }

        VulkanSemaphore(VulkanContext* context, VkSemaphore semaphore)
            : _context(context), _semaphore(semaphore) {
        }

        void initialize();
        void destroy();

        VkSemaphore vkSemaphore() const;

        operator VkSemaphore() const;

    };
} // namespace nwt
