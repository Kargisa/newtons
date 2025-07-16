#pragma once

#include "newtons/pch.hpp"
#include "vulkan/vulkan.h"

namespace nwt
{
    struct VulkanContext;

    struct VulkanQueueInfo
    {
        uint32_t family;
        uint32_t index;

        VulkanQueueInfo()
            : family(std::numeric_limits<uint32_t>().max()), index(std::numeric_limits<uint32_t>().max()) {
        }

        VulkanQueueInfo(uint32_t family, uint32_t index)
            : family(family), index(index) {
        }
    };

    class VulkanQueue
    {
        VulkanContext* _context;
        VkQueue _queue;
        VulkanQueueInfo _queueInfo;

    public:
        VulkanQueue()
            : _context(nullptr), _queue(VK_NULL_HANDLE), _queueInfo(VulkanQueueInfo()) {
        }

        VulkanQueue(VulkanContext* context, VulkanQueueInfo queueInfo)
            : _context(context), _queue(VK_NULL_HANDLE), _queueInfo(queueInfo) {
        }

        VulkanQueue(VulkanContext* context, VkQueue queue, VulkanQueueInfo queueInfo)
            : _context(context), _queue(queue), _queueInfo(queueInfo) {
        }

        void initialize();

        VulkanQueueInfo info() const;
        void submit(const std::vector<VkCommandBuffer>& commandBuffers, const std::vector<VkSemaphore>& waitSemaphores, const std::vector<VkPipelineStageFlags>& waitStages, const std::vector<VkSemaphore>& signalSemaphores, const VkFence& fence) const;

        VkQueue vkQueue() const;
        operator VkQueue() const;
    };
} // namespace nwt
