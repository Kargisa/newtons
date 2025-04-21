#pragma once

#include <vulkan/vulkan.h>

#include "newtons/pch.hpp"
#include "vulkanDevice.hpp"

namespace nwt
{
    struct VulkanContext;

    class VulkanPhysicalDevice {
    private:
        VulkanContext* _context;
        VkPhysicalDevice _physicalDevice;
        uint32_t _physicalDeviceRating;
        // TODO: Logical Devices
        std::vector<VulkanDevice> _devices;
    public:
        VulkanPhysicalDevice(VulkanContext* context)
            : _context(context) {
        }
        ~VulkanPhysicalDevice();

        uint32_t getPhysicalDeviceRating() const;
        std::vector<VkQueueFamilyProperties> getAvailableQueueFamilyProperties();
    private:
        void ratePhysicalDevice();
    };

} // namespace nwt
