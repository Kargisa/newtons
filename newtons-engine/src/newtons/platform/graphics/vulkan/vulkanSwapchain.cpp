#include "vulkanSwapchain.hpp"
#include "vulkanContext.hpp"



namespace nwt
{
    VulkanSwapchain::~VulkanSwapchain() {
        destroy();
    }

    VkSwapchainKHR VulkanSwapchain::getVkSwapchain() const {
        return _vkSwapchain;
    }

    VkExtent2D VulkanSwapchain::getExtent() const {
        return _extent;
    }

    VkFormat VulkanSwapchain::getImageFormat() const {
        return _imageFormat;
    }

    const std::vector<VkImage>& VulkanSwapchain::getImages() const {
        return _images;
    }

    const std::vector<VkImageView>& VulkanSwapchain::getImageViews() const {
        return _imageViews;
    }

    bool VulkanSwapchain::isFramebufferResized() const {
        return _framebufferResized;
    }

    VkResult VulkanSwapchain::create() {
        VkDevice device = _context->getDevice();

        SupportDetails swapChainDetails = querySupportDetails();

        VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(swapChainDetails.formats);
        VkPresentModeKHR presentMode = choosePresentMode(swapChainDetails.presentModes);
        VkExtent2D extent = chooseExtent(swapChainDetails.capabilities);

        uint32_t imgCount = swapChainDetails.capabilities.minImageCount + 1;
        if (swapChainDetails.capabilities.maxImageCount > 0 && imgCount > swapChainDetails.capabilities.maxImageCount) {
            imgCount = swapChainDetails.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = _context->getSurface();
        createInfo.minImageCount = imgCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        VulkanQueueFamilyIndices indices = _context->findQueueFamilies();

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainDetails.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &_vkSwapchain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create the swap chain");
        }

        vkGetSwapchainImagesKHR(device, _vkSwapchain, &imgCount, nullptr);
        _images.resize(imgCount);
        vkGetSwapchainImagesKHR(device, _vkSwapchain, &imgCount, _images.data());

        _imageFormat = surfaceFormat.format;
        _extent = extent;
    }

    VulkanSwapchain::SupportDetails VulkanSwapchain::querySupportDetails() {
        VkPhysicalDevice device = _context->getPhysicalDevice();
        VkSurfaceKHR surface = _context->getSurface();
        return querySupportDetails(device, surface);
    }

    VulkanSwapchain::SupportDetails VulkanSwapchain::querySupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface) {
        VulkanSwapchain::SupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    void VulkanSwapchain::createImageViews() {
        _imageViews.resize(_images.size());

        for (size_t i = 0; i < _images.size(); i++)
        {
            _imageViews[i] = _context->createImageView(_images[i], _imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }

    void VulkanSwapchain::destroy() {
    }


} // namespace nwt