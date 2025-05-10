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

        VulkanQueueInfo()
            : family(-1), index(-1) {
        }

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
        [0]: Present
        [1]: Graphics
        [2]: Transfer
        [3]: Compute
        */
        std::vector<VulkanQueueInfo> _queueInfos;

        /*
        Reserved Queue Indices:
        [0]: Present
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
        // ~VulkanDevice();

        VulkanDevice(const VulkanDevice& other) = delete;

        static VulkanDevice* create(VulkanContext* context, VulkanPhysicalDevice* physicalDevice);
        void initialize();
        void destroy();


        VkDevice vkDevice() const;

        VkQueue vkPresentQueue() const;
        VkQueue vkGraphicsQueue() const;
        VkQueue vkTransferQueue() const;
        VkQueue vkComputeQueue() const;

        const VulkanQueueInfo& presentQueueInfo() const;
        const VulkanQueueInfo& graphicsQueueInfo() const;
        const VulkanQueueInfo& transferQueueInfo() const;
        const VulkanQueueInfo& computeQueueInfo() const;

    private:
        VulkanQueueInfo findPresentQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps);
        VulkanQueueInfo findGraphicsQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps);
        VulkanQueueInfo findTransferQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps);
    };

} // namespace nwt
