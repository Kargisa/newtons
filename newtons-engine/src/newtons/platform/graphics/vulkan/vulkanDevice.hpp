#pragma once

#include <vulkan/vulkan.h>

#include "newtons/pch.hpp"
#include "vulkanPhysicalDevice.hpp"

namespace nwt
{
    struct VulkanContext;

    struct VulkanQueueInfo
    {
        uint32_t family;
        uint32_t index;

        VulkanQueueInfo(uint32_t family, uint32_t index)
            : family(family), index(index) {
        }
    };

    class VulkanDevice
    {

    private:

        VulkanContext* _context;
        VulkanPhysicalDevice* _physicalDevice;
        VkDevice _device;

        /*
        Reserved Queue Indices:
        [0]: Presentation
        [1]: Graphics
        [2]: Transfer
        [3]: Compute
        */
        std::vector<VkQueue> _queues;

    public:
        VulkanDevice(VulkanContext* context, VulkanPhysicalDevice* physicalDevice)
            : _context(context), _physicalDevice(physicalDevice), _device(nullptr) {
        }

        VulkanDevice()
            : _context(nullptr), _physicalDevice(nullptr), _device(nullptr) {
        }

        ~VulkanDevice();

        static VulkanDevice* create(VulkanContext* context, VulkanPhysicalDevice* physicalDevice);
        void init();
        void destroy();

        VulkanQueueInfo findPresentQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps, const std::vector<VulkanQueueInfo>& usedQueues);
        VulkanQueueInfo findGraphicsQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps, const std::vector<VulkanQueueInfo>& usedQueues);

        VkDevice getVkDevice() const;

        VkQueue getVkPresentQueue() const;
        VkQueue getVkGraphicsQueue() const;
        VkQueue getVkComputeQueue() const;
        VkQueue getVkTransfereQueue() const;
    };

} // namespace nwt
