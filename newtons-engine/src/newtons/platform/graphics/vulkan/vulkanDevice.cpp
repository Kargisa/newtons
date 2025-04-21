#include "vulkanDevice.hpp"
#include "vulkanContext.hpp"

namespace nwt
{

    void VulkanDevice::create(VulkanDeviceConfiguration config, VkPhysicalDevice physicalDevice, const std::vector<VkQueueFamilyProperties> availableQueueFamilyProps) {
        // VulkanQueueFamilyIndices indices = findQueueFamilies(physicalDevice);

        // std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        // std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        float queuePriority = VK_QUEUE_GLOBAL_PRIORITY_MEDIUM_EXT;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            queueCreateInfos.push_back(queueCreateInfo);
        }

        // VkDeviceQueueCreateInfo queueCreateInfo = {};
        // queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        // queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        // queueCreateInfo.queueCount = 1;

        // float queuePriority = 1.0f;
        // queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;


        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        // if constexpr (_enableValidationLayers) {
        //     createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
        //     createInfo.ppEnabledLayerNames = _validationLayers.data();
        // }
        // else {
        //     createInfo.enabledLayerCount = 0;
        // }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);
        vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &_presentQueue);
        switch (config)
        {
        case VulkanDeviceConfiguration::DEAFAULT:
            break;
        default:
            std::runtime_error("no such vulkan device configuration");
            break;
        }
    }

} // namespace nwt
