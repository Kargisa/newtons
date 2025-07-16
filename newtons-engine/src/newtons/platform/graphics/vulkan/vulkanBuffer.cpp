#include "vulkanBuffer.hpp"
#include "vulkanContext.hpp"

namespace nwt
{
    VkBuffer VulkanBuffer::vkBuffer() const {
        return _buffer;
    }

    VulkanBuffer::operator VkBuffer() const {
        return _buffer;
    }

    VmaAllocation VulkanBuffer::vmaAllocation() const {
        return _allocation;
    }

    VmaAllocationInfo2 VulkanBuffer::vmaAllocationInfo() const {
        VmaAllocationInfo2 info;
        vmaGetAllocationInfo2(_context->vmaAllocator(), _allocation, &info);
        return info;
    }

    void VulkanBuffer::initialize(VkDeviceSize bufferSize, VkBufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags allocationFlags) {
        VkBufferCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.size = bufferSize;
        createInfo.usage = bufferUsage;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = memoryUsage;
        allocInfo.flags = allocationFlags;

        VmaAllocationInfo info;
        if (vmaCreateBuffer(_context->vmaAllocator(), &createInfo, &allocInfo, &_buffer, &_allocation, &info) != VK_SUCCESS) {
            LOG_FAIL("Could not create VulkanBuffer");
            throw std::runtime_error("");
        }
    }

    void VulkanBuffer::initialize(VulkanBufferType type, VkDeviceSize bufferSize) {
        switch (type)
        {
        case VulkanBufferType::STAGING_BUFFER:
            initialize(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_AUTO, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
            break;
        default:
            LOG_FAIL("No VulkanBufferType of " << static_cast<int>(type) << ". Could not create Buffer");
            throw std::runtime_error("");
        }
    }

    void VulkanBuffer::destroy() {
        if (_buffer == VK_NULL_HANDLE || _allocation == nullptr) {
            return;
        }

        vmaDestroyBuffer(_context->vmaAllocator(), _buffer, _allocation);
    }
} // namespace nwt
