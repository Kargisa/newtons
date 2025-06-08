#include "vulkanCommandBuffer.hpp"
#include "vulkanCommandPool.hpp"
#include "vulkanContext.hpp"

namespace nwt
{
    void VulkanCommandBuffer::initialize(const VulkanCommandPool& pool) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = pool.vkCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(_context->device().vkDevice(), &allocInfo, &_buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        LOG_INFO("Command Buffer Successfully Created!");
    }

    void VulkanCommandBuffer::reset(VkCommandBufferResetFlags flags) const {
        vkResetCommandBuffer(_buffer, flags);
    }

    void VulkanCommandBuffer::begin() const {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(_buffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
    }

    void VulkanCommandBuffer::end() const {
        vkEndCommandBuffer(_buffer);
    }

    void VulkanCommandBuffer::setVkCommandBuffer(VkCommandBuffer commandBuffer) {
        _buffer = commandBuffer;
    }

    VkCommandBuffer VulkanCommandBuffer::vkCommandBuffer() const {
        return _buffer;
    }

    VulkanCommandBuffer::operator VkCommandBuffer() const {
        return _buffer;
    }
} // namespace nwt
