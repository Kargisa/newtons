#pragma once

#include <vulkan/vulkan.h>

#include "newtons/pch.hpp"

namespace nwt
{
    struct VulkanContext;

    enum class VulkanDeviceConfiguration
    {
        DEAFAULT = 0
    };

    class VulkanDevice
    {
    private:
        VulkanContext* _context;
        VkDevice _device;
        std::vector<VkQueue> _queues;

    public:
        VulkanDevice(VulkanContext* context)
            : _context(context) {
        }
        ~VulkanDevice();

        void create(VulkanDeviceConfiguration config, VkPhysicalDevice physicalDevice, const std::vector<VkQueueFamilyProperties> availableQueueFamilyProps);

        VkQueue getPresentQueue() const;
        VkQueue getGraphicsQueue() const;
        VkQueue getComputeQueue() const;
        VkQueue getTransfereQueue() const;
    };

} // namespace nwt
