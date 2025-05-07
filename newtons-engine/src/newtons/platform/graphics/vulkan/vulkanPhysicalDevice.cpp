#include "vulkanPhysicalDevice.hpp"
#include "vulkanContext.hpp"

namespace nwt
{

    const std::vector<const char*> VulkanPhysicalDevice::_deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkPhysicalDevice VulkanPhysicalDevice::getVkPhysicalDevice() const {
        return _vkPhysicalDevice;
    }

    uint32_t VulkanPhysicalDevice::getRating() const {
        VkPhysicalDeviceProperties deviceProps;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(_vkPhysicalDevice, &deviceProps);
        vkGetPhysicalDeviceFeatures(_vkPhysicalDevice, &deviceFeatures);

        bool isSwapchainAdequate = false;
        if (hasRequiredExtensionSupport(_deviceExtensions)) {
            VulkanSwapchainSupportDetails swapDetails = querySwapchainSupportDetails();
            isSwapchainAdequate = !swapDetails.formats.empty() && !swapDetails.presentModes.empty();
        }

        if (!isSwapchainAdequate) {
            return 0;
        }

        //TODO: check for queries and add score for each existing query

        int score = 0;
        if (deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        score += deviceProps.limits.maxImageDimension2D;
        score += deviceProps.limits.maxImageDimension3D;

        return score;
    }

    const std::vector<const char*>& VulkanPhysicalDevice::getDeviceExtensions() {
        return _deviceExtensions;
    }

    VulkanSwapchainSupportDetails VulkanPhysicalDevice::querySwapchainSupportDetails() const {
        VulkanSwapchainSupportDetails details = {};

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_vkPhysicalDevice, _context->getVkSurface(), &details.capabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(_vkPhysicalDevice, _context->getVkSurface(), &formatCount, nullptr);
        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(_vkPhysicalDevice, _context->getVkSurface(), &formatCount, details.formats.data());
        }

        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(_vkPhysicalDevice, _context->getVkSurface(), &presentModeCount, nullptr);
        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(_vkPhysicalDevice, _context->getVkSurface(), &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    bool VulkanPhysicalDevice::hasRequiredExtensionSupport(const std::vector<const char*>& extensions) const {
        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(_vkPhysicalDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(_vkPhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    std::vector<VkQueueFamilyProperties> VulkanPhysicalDevice::getAvailableQueueFamilyProperties() const {
        uint32_t count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, &count, nullptr);
        std::vector<VkQueueFamilyProperties> queueFmilies(count);

        vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, &count, queueFmilies.data());

        return queueFmilies;
    }
} // namespace nwt
