
#include "vulkanContext.hpp"
#include "vulkanFence.hpp"

namespace nwt
{

    void VulkanFence::initialize(bool signaled) {
        VkFenceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        createInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

        if (vkCreateFence(_context->device().vkDevice(), &createInfo, nullptr, &_fence) != VK_SUCCESS) {
            std::runtime_error("Could not create fence!");
        }
    }

    void VulkanFence::destroy() {
        if (_fence == VK_NULL_HANDLE) {
            return;
        }

        vkDestroyFence(_context->device().vkDevice(), _fence, nullptr);
        LOG_INFO("Fence Destroyed!");
    }

    void VulkanFence::wait(uint64_t timeout) const {
        vkWaitForFences(_context->device().vkDevice(), 1, &_fence, VK_TRUE, timeout);
    }

    void VulkanFence::reset() const {
        vkResetFences(_context->device().vkDevice(), 1, &_fence);
    }

    VkFence VulkanFence::vkFence() const {
        return _fence;
    }

    VulkanFence::operator VkFence() const {
        return _fence;
    }


} // namespace nwt
