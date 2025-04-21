#include "vulkanPhysicalDevice.hpp"
#include "vulkanContext.hpp"

namespace nwt
{
    std::vector<VkQueueFamilyProperties> VulkanPhysicalDevice::getAvailableQueueFamilyProperties()
    {
        uint32_t count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &count, nullptr);

        std::vector<VkQueueFamilyProperties> queueFmilies;
        queueFmilies.reserve(count);
        vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &count, queueFmilies.data());

        return queueFmilies;
    }


} // namespace nwt
