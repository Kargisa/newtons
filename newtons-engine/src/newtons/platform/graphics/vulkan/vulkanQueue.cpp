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

    void VulkanQueue::submit(const std::vector<VulkanCommandBuffer>& commandBuffers, const std::vector<VulkanSemaphore>& waitSemaphores, const std::vector<VkPipelineStageFlags>& waitStages, std::vector<VulkanSemaphore> signalSemaphores, const VulkanFence& fence) const {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        std::vector<VkSemaphore> vkWaitSemaphores(waitSemaphores.size());
        for (size_t i = 0; i < vkWaitSemaphores.size(); i++) {
            vkWaitSemaphores[i] = waitSemaphores[i];
        }

        submitInfo.waitSemaphoreCount = vkWaitSemaphores.size();
        submitInfo.pWaitSemaphores = vkWaitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages.data();


        std::vector<VkCommandBuffer> vkCommandBuffers(commandBuffers.size());
        for (size_t i = 0; i < vkCommandBuffers.size(); i++) {
            vkCommandBuffers[i] = commandBuffers[i];
        }

        submitInfo.commandBufferCount = vkCommandBuffers.size();
        submitInfo.pCommandBuffers = vkCommandBuffers.data();


        std::vector<VkSemaphore> vkSignalSemaphores(signalSemaphores.size());
        for (size_t i = 0; i < vkSignalSemaphores.size(); i++) {
            vkSignalSemaphores[i] = signalSemaphores[i];
        }

        submitInfo.signalSemaphoreCount = vkSignalSemaphores.size();
        submitInfo.pSignalSemaphores = vkSignalSemaphores.data();

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
