#pragma once

#include <vulkan/vulkan.h>

#include "newtons/pch.hpp"
// #include "vulkanDevice.hpp"

namespace nwt
{
    struct VulkanContext;

    struct VulkanSwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanPhysicalDevice {
    private:
        VulkanContext* _context;
        VkPhysicalDevice _vkPhysicalDevice;


        static const std::vector<const char*> _deviceExtensions;

    public:
        VulkanPhysicalDevice(VulkanContext* context, VkPhysicalDevice vkPhysicalDevice)
            : _context(context), _vkPhysicalDevice(vkPhysicalDevice) {
        }

        VulkanPhysicalDevice()
            : _context(nullptr), _vkPhysicalDevice(VK_NULL_HANDLE) {
        }

        VkPhysicalDevice vkPhysicalDevice() const;
        operator VkPhysicalDevice() const;

        uint32_t getRating() const;
        static const std::vector<const char*>& getDeviceExtensions();

        VulkanSwapchainSupportDetails querySwapchainSupportDetails() const;
        bool hasRequiredExtensionSupport(const std::vector<const char*>& deviceExtensions) const;
        std::vector<VkQueueFamilyProperties> getAvailableQueueFamilyProperties() const;
    };

} // namespace nwt
