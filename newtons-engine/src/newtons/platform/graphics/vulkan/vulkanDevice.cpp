#include "vulkanDevice.hpp"
#include "vulkanContext.hpp"

namespace nwt
{

    void VulkanDevice::create() {
        // VulkanQueueFamilyIndices indices = findQueueFamilies(physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

        std::vector<VkQueueFamilyProperties> queueFamilyProps = _physicalDevice->getAvailableQueueFamilyProperties();

        std::vector<VulkanQueueInfo> queueInfos;
        queueInfos.emplace_back(findPresentQueueInfo(queueFamilyProps, queueInfos));
        queueInfos.emplace_back(findGraphicsQueueInfo(queueFamilyProps, queueInfos));


        std::unordered_map<uint32_t, uint32_t> uniqueQueueFamilies;
        for (auto&& queueInfo : queueInfos) {
            if (uniqueQueueFamilies.try_emplace(queueInfo.family).second) {
                uniqueQueueFamilies[queueInfo.family] = 1;
                continue;
            }
            uniqueQueueFamilies[queueInfo.family]++;
        }


        float queuePriority = VK_QUEUE_GLOBAL_PRIORITY_MEDIUM_EXT;
        for (auto&& queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily.first;
            queueCreateInfo.queueCount = queueFamily.second;
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

        createInfo.enabledExtensionCount = static_cast<uint32_t>(VulkanPhysicalDevice::getDeviceExtensions().size());
        createInfo.ppEnabledExtensionNames = VulkanPhysicalDevice::getDeviceExtensions().data();

        // if constexpr (_enableValidationLayers) {
        //     createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
        //     createInfo.ppEnabledLayerNames = _validationLayers.data();
        // }
        // else {
        //     createInfo.enabledLayerCount = 0;
        // }

        if (vkCreateDevice(_physicalDevice->getVkPhysicalDevice(), &createInfo, nullptr, &_device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(_device, queueInfos[0].family, queueInfos[0].index, &_queues[0]);
        vkGetDeviceQueue(_device, queueInfos[1].family, queueInfos[1].index, &_queues[1]);
    }

    VulkanQueueInfo VulkanDevice::findPresentQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps, const std::vector<VulkanQueueInfo>& usedQueues) {
        VulkanQueueInfo queueInfo(-1, 0);

        uint32_t savedFamily = -1;
        uint32_t familyIndex = 0;
        for (auto&& familyProp : availableQueueFamilyProps)
        {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice->getVkPhysicalDevice(), familyIndex, _context->getVkSurface(), &presentSupport);

            // checks if queue family is able present to the surface
            if (!presentSupport || (familyProp.queueFlags == VK_QUEUE_TRANSFER_BIT) || (familyProp.queueFlags == VK_QUEUE_COMPUTE_BIT)) {
                familyIndex++;
                continue;
            }

            // checks id the current queue family is used by any of the "usedQueues"
            // if not, select current queue family at index 0
            bool familyUsed = false;
            for (auto&& usedQueue : usedQueues) {
                if (usedQueue.family == familyIndex) {
                    familyUsed = true;
                    break;
                }
            }

            if (!familyUsed) {
                queueInfo.family = familyIndex;
                return queueInfo;
            }

            // puts all queue indices of "usedQueues" that have the save queue family as the current queue family in an "unordered_set"
            std::unordered_set<uint32_t> usedQueueIndices;
            for (auto&& usedQueue : usedQueues) {
                if (usedQueue.family == familyIndex) {
                    usedQueueIndices.emplace(usedQueue.index);
                }
            }

            // checks for availavle queue indices in the set
            // if an index is free, select current queue family and free queue index
            for (uint32_t i = 0; i < familyProp.queueCount; i++) {
                if (!usedQueueIndices.contains(i)) {
                    queueInfo.family = familyIndex;
                    queueInfo.index = i;
                    return queueInfo;
                }
            }

            // select current queue family as fallback if none was selected
            if (savedFamily == -1) {
                savedFamily = familyIndex;
            }
            familyIndex++;
        }

        // fallback if no unique queue family was found
        if (queueInfo.family == -1) {
            queueInfo.family = savedFamily;
        }

        return queueInfo;
    }

    VulkanQueueInfo VulkanDevice::findGraphicsQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps, const std::vector<VulkanQueueInfo>& usedQueues) {
        VulkanQueueInfo queueInfo(-1, 0);

        uint32_t savedFamily = -1;
        uint32_t familyIndex = 0;
        for (auto&& familyProp : availableQueueFamilyProps)
        {
            VkBool32 presentSupport = false;

            // checks if queue family is able present to the surface
            if (!(familyProp.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                familyIndex++;
                continue;
            }

            // checks id the current queue family is used by any of the "usedQueues"
            // if not, select current queue family at index 0
            bool familyUsed = false;
            for (auto&& usedQueue : usedQueues) {
                if (usedQueue.family == familyIndex) {
                    familyUsed = true;
                    break;
                }
            }

            if (!familyUsed) {
                queueInfo.family = familyIndex;
                return queueInfo;
            }

            // puts all queue indices of "usedQueues" that have the save queue family as the current queue family in an "unordered_set"
            std::unordered_set<uint32_t> usedQueueIndices;
            for (auto&& usedQueue : usedQueues) {
                if (usedQueue.family == familyIndex) {
                    usedQueueIndices.emplace(usedQueue.index);
                }
            }

            // checks for availavle queue indices in the set
            // if an index is free, select current queue family and free queue index
            for (uint32_t i = 0; i < familyProp.queueCount; i++) {
                if (!usedQueueIndices.contains(i)) {
                    queueInfo.family = familyIndex;
                    queueInfo.index = i;
                    return queueInfo;
                }
            }

            // select current queue family as fallback if none was selected
            if (savedFamily == -1) {
                savedFamily = familyIndex;
            }
            familyIndex++;
        }

        // fallback if no unique queue family was found
        if (queueInfo.family == -1) {
            queueInfo.family = savedFamily;
        }

        return queueInfo;
    }

    VkDevice VulkanDevice::getVkDevice() const {
        return _device;
    }

} // namespace nwt
