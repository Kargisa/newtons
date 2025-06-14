#pragma once

#include <vulkan/vulkan.h>

#include "newtons/pch.hpp"
#include "vulkanPhysicalDevice.hpp"

namespace nwt
{
    struct VulkanContext;

    class VulkanDevice
    {

    private:

        VulkanContext* _context;
        VulkanPhysicalDevice* _physicalDevice;
        VkDevice _device;

    public:
        VulkanDevice(VulkanContext* context, VulkanPhysicalDevice* physicalDevice)
            : _context(context), _physicalDevice(physicalDevice), _device(VK_NULL_HANDLE) {
        }
        VulkanDevice()
            : _context(nullptr), _physicalDevice(nullptr), _device(VK_NULL_HANDLE) {
        }
        // ~VulkanDevice();

        VulkanDevice(const VulkanDevice& other) = delete;

        static VulkanDevice* create(VulkanContext* context, VulkanPhysicalDevice* physicalDevice);
        void initialize();
        void destroy();

        void waitIdle() const;

        VkDevice vkDevice() const;
        operator VkDevice() const;
    private:
    };

} // namespace nwt
