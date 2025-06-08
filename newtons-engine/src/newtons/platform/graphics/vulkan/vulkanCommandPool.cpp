#include "vulkanCommandPool.hpp"
#include "vulkanContext.hpp"

namespace nwt
{
    VkCommandPool VulkanCommandPool::vkCommandPool() const {
        return _pool;
    }

    void VulkanCommandPool::initialize(uint32_t queueFamily, VkCommandPoolCreateFlags flags) {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = flags;
        poolInfo.queueFamilyIndex = queueFamily;

        VkCommandPool pool = VK_NULL_HANDLE;
        if (vkCreateCommandPool(_context->device().vkDevice(), &poolInfo, nullptr, &_pool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }


        LOG_INFO("Command Pool Successfully Created!");
    }

    void VulkanCommandPool::destroy() {
        if (_pool == VK_NULL_HANDLE) {
            return;
        }

        vkDestroyCommandPool(_context->device().vkDevice(), _pool, nullptr);

        LOG_INFO("Command Pool Destroyed!");
    }

    std::vector<VulkanCommandBuffer> VulkanCommandPool::createCommandBuffers(uint32_t count, VkCommandBufferLevel level) const
    {
        FixedVector<VkCommandBuffer> vkBuffers(count);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = _pool;
        allocInfo.level = level;
        allocInfo.commandBufferCount = count;

        if (vkAllocateCommandBuffers(_context->device().vkDevice(), &allocInfo, vkBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        std::vector<VulkanCommandBuffer> _buffers;
        _buffers.reserve(vkBuffers.size());

        for (size_t i = 0; i < vkBuffers.size(); i++) {
            _buffers.emplace_back(_context, vkBuffers[i]);
        }

        return _buffers;
    }


} // namespace nwt
