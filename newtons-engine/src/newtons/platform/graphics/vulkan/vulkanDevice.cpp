#include "vulkanDevice.hpp"
#include "vulkanContext.hpp"
#include <bitset>
#include <newtons/logging/log.hpp>

namespace nwt
{
    // VulkanDevice::~VulkanDevice() {
    //     destroy();
    // }

    VulkanDevice* VulkanDevice::create(VulkanContext* context, VulkanPhysicalDevice* physicalDevice) {
        VulkanDevice* device = new VulkanDevice(context, physicalDevice);
        return device;
    }

    void VulkanDevice::initialize() {
        //         std::vector<VkQueueFamilyProperties> queueFamilyProps = _physicalDevice->getAvailableQueueFamilyProperties();

        //         int iii = 0;
        //         LOG_INFO("Graphics Bits: " << std::bitset<32>(VK_QUEUE_GRAPHICS_BIT));
        //         LOG_INFO("Transfer Bits: " << std::bitset<32>(VK_QUEUE_TRANSFER_BIT));
        //         LOG_INFO("Compute Bits:  " << std::bitset<32>(VK_QUEUE_COMPUTE_BIT));
        //         LOG_INFO("----------------");

        //         for (auto&& prop : queueFamilyProps) {
        //             std::bitset<32> set(prop.queueFlags);
        //             LOG_INFO(iii << ") " << set << ", queue count: " << prop.queueCount);
        //             iii++;
        //         }

        //         LOG_INFO("----------------");

        // #ifdef DEBUG
        //         int qi = 0;
        //         for (auto&& queueInfo : _queueInfos) {
        //             LOG_INFO(qi << ") family: " << queueInfo.family << ", index: " << queueInfo.index);
        //             qi++;
        //         }
        // #endif

        std::unordered_map<uint32_t, uint32_t> uniqueQueueFamilies;
        std::unordered_map<uint32_t, std::vector<uint32_t>> usedQueuesInFamily;
        for (auto&& queueInfo : _context->queues()) {
            if (uniqueQueueFamilies.try_emplace(queueInfo.info().family).second) {
                uniqueQueueFamilies[queueInfo.info().family] = 1;
                usedQueuesInFamily[queueInfo.info().family].emplace_back(queueInfo.info().index);
                continue;
            }

            bool queueUsed = false;
            for (auto&& usedQueue : usedQueuesInFamily[queueInfo.info().family]) {
                if (usedQueue == queueInfo.info().index) {
                    queueUsed = true;
                    break;
                }
            }

            if (!queueUsed) {
                uniqueQueueFamilies[queueInfo.info().family]++;
            }
        }


        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

        std::vector<std::vector<float>> queuePriorities;
        for (auto&& queueFamily : uniqueQueueFamilies) {
            queuePriorities.emplace_back(queueFamily.second, 1.0f);

            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily.first;
            queueCreateInfo.queueCount = queueFamily.second;
            queueCreateInfo.pQueuePriorities = queuePriorities.back().data();

            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(VulkanPhysicalDevice::getDeviceExtensions().size());
        createInfo.ppEnabledExtensionNames = VulkanPhysicalDevice::getDeviceExtensions().data();

        if constexpr (VulkanContext::validationLayersEnabled()) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(_context->validationLayers().size());
            createInfo.ppEnabledLayerNames = _context->validationLayers().data();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }


        if (vkCreateDevice(_physicalDevice->vkPhysicalDevice(), &createInfo, nullptr, &_device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }
    }

    void VulkanDevice::destroy() {
        if (_device == VK_NULL_HANDLE) {
            return;
        }

        vkDestroyDevice(_device, nullptr);
        _device = VK_NULL_HANDLE;
        _context = nullptr;
        _physicalDevice = nullptr;

        LOG_INFO("Logical Device Destroyed!\n");
    }

    VkDevice VulkanDevice::vkDevice() const {
        return _device;
    }

    VulkanDevice::operator VkDevice() const {
        return _device;
    }


} // namespace nwt
