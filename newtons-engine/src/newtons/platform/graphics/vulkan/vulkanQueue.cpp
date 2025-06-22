#include "vulkanQueue.hpp"
#include "vulkanContext.hpp"

namespace nwt
{
    void VulkanQueue::initialize() {
        vkGetDeviceQueue(_context->device().vkDevice(), _queueInfo.family, _queueInfo.index, &_queue);
        LOG_INFO("Queueu Successfully Created! " << _queue);
    }

    VulkanQueueInfo VulkanQueue::info() const {
        return _queueInfo;
    }

    void VulkanQueue::submit(const std::vector<VkCommandBuffer>& commandBuffers, const std::vector<VkSemaphore>& waitSemaphores, const std::vector<VkPipelineStageFlags>& waitStages, std::vector<VkSemaphore> signalSemaphores, const VkFence& fence) const {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = waitSemaphores.size();
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages.data();

        submitInfo.commandBufferCount = commandBuffers.size();
        submitInfo.pCommandBuffers = commandBuffers.data();

        submitInfo.signalSemaphoreCount = signalSemaphores.size();
        submitInfo.pSignalSemaphores = signalSemaphores.data();

        if (vkQueueSubmit(_queue, 1, &submitInfo, fence) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit queue!");
        }
    }

    VkQueue VulkanQueue::vkQueue() const {
        return _queue;
    }

    VulkanQueue::operator VkQueue() const {
        return _queue;
    }
} // namespace nwt
