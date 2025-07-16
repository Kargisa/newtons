#pragma once

#include "newtons/pch.hpp"
#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

namespace nwt
{
    struct VulkanContext;

    enum class VulkanBufferType {
        STAGING_BUFFER,
    };

    class VulkanBuffer {
        VulkanContext* _context;
        VkBuffer _buffer;
        VmaAllocation _allocation;

    public:
        VulkanBuffer()
            : _context(nullptr), _allocation(nullptr), _buffer(VK_NULL_HANDLE) {
        }

        VulkanBuffer(VulkanContext* context)
            : _context(context), _allocation(nullptr), _buffer(VK_NULL_HANDLE) {
        }

        VkBuffer vkBuffer() const;
        operator VkBuffer() const;

        VmaAllocation vmaAllocation() const;
        VmaAllocationInfo2 vmaAllocationInfo() const;

        void initialize(VkDeviceSize bufferSize, VkBufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags allocationFlags);
        void initialize(VulkanBufferType type, VkDeviceSize bufferSize);
        void destroy();
    };
} // namespace nwt
