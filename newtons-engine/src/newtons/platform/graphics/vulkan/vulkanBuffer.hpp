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
        std::atomic<size_t>* _atomicCount;
        VkBuffer _buffer;
        VmaAllocation _allocation;
        VkDeviceSize _size;

        // INFO: Debug
        // std::string name;

    public:
        VulkanBuffer()
            : _context(nullptr), _size(0), _allocation(nullptr), _buffer(VK_NULL_HANDLE), _atomicCount(new std::atomic<size_t>(1)) {
        }

        VulkanBuffer(VulkanContext* context, VkDeviceSize size)
            : _context(context), _size(size), _allocation(nullptr), _buffer(VK_NULL_HANDLE), _atomicCount(new std::atomic<size_t>(1)) {
        }

        VulkanBuffer(const VulkanBuffer& other)
            : _context(other._context), _size(other._size), _atomicCount(other._atomicCount), _buffer(other._buffer), _allocation(other._allocation) {
            addAtomicCount();
        }

        VulkanBuffer(VulkanBuffer&& other) noexcept = delete;

        ~VulkanBuffer();


        VulkanBuffer& operator=(const VulkanBuffer& other);

        VkBuffer vkBuffer() const;
        operator VkBuffer() const;

        VkDeviceSize size() const;

        VmaAllocation vmaAllocation() const;
        VmaAllocationInfo2 vmaAllocationInfo() const;

        void initialize(VkBufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags allocationFlags);
        void initialize(VulkanBufferType type);
        void destroy();

        VkResult mapMemory(void** ppData) const;
        void unmapMemory() const;

        size_t atomicCount() const;

    private:
        void addAtomicCount();
        void subAtomicCount();
    };

    struct VulkanBufferCopyInfo {
        const VulkanBuffer srcBuffer;
        const VulkanBuffer dstBuffer;
        VkPipelineStageFlags dstStageMask;
        VkAccessFlags dstAccessMask;

        VulkanBufferCopyInfo() = default;
        VulkanBufferCopyInfo(const VulkanBuffer srcBuffer, const VulkanBuffer dstBuffer, VkPipelineStageFlags dstUsageFlag, VkAccessFlags dstAccessMask)
            : srcBuffer(srcBuffer), dstBuffer(dstBuffer), dstStageMask(dstUsageFlag), dstAccessMask(dstAccessMask) {
        }

    };
} // namespace nwt
