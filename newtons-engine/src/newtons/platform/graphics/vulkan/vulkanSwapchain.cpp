#include "vulkanSwapchain.hpp"
#include "vulkanContext.hpp"
#include "newtons/application.hpp"



namespace nwt
{
    VulkanSwapchain& VulkanSwapchain::operator=(const VulkanSwapchain& other) {
        if (this == &other) {
            return *this;
        }

        _context = other._context;
        _swapchain = other._swapchain;
        _extent = other._extent;
        _imageFormat = other._imageFormat;
        _images = other._images;
        _imageViews = other._imageViews;
        return *this;
    }

    VkSwapchainKHR VulkanSwapchain::vkSwapchain() const {
        return _swapchain;
    }

    VulkanSwapchain::operator VkSwapchainKHR() const {
        return _swapchain;
    }

    VkFormat VulkanSwapchain::vkImageFormat() const {
        return _imageFormat;
    }

    const VkExtent2D& VulkanSwapchain::vkExtent() const {
        return _extent;
    }

    const FixedVector<VkImage>& VulkanSwapchain::vkImages() const {
        return _images;
    }

    const FixedVector<VkImageView>& VulkanSwapchain::vkImageViews() const {
        return _imageViews;
    }

    void VulkanSwapchain::initialize() {
        const VulkanDevice& device = _context->device();

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
        createInfo.surface = _context->vkSurface();
        createInfo.minImageCount = imgCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        VulkanQueueInfo graphicsQueueInfo = _context->graphicsQueue().info();
        VulkanQueueInfo presentQueueInfo = _context->presentQueue().info();

        if (graphicsQueueInfo.family != presentQueueInfo.family) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            uint32_t queueFamilyIndices[] = { graphicsQueueInfo.family, presentQueueInfo.family };
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


        if (vkCreateSwapchainKHR(device.vkDevice(), &createInfo, nullptr, &_swapchain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swapchain");
        }

        vkGetSwapchainImagesKHR(device.vkDevice(), _swapchain, &imgCount, nullptr);
        _images = FixedVector<VkImage>(imgCount);
        vkGetSwapchainImagesKHR(device.vkDevice(), _swapchain, &imgCount, _images.data());

        _imageFormat = surfaceFormat.format;
        _extent = extent;


        _imageViews = FixedVector<VkImageView>(_images.size());
        for (size_t i = 0; i < _images.size(); i++)
        {
            _imageViews[i] = _context->createImageView(_images[i], _imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }

    VulkanSwapchain::SupportDetails VulkanSwapchain::querySupportDetails() {
        VulkanSwapchain::SupportDetails details;

        VkPhysicalDevice phDevice = _context->physicalDevice().vkPhysicalDevice();
        VkSurfaceKHR surface = _context->vkSurface();

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phDevice, surface, &details.capabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(phDevice, surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(phDevice, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(phDevice, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(phDevice, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }


    VkSurfaceFormatKHR VulkanSwapchain::chooseSurfaceFormat(std::vector<VkSurfaceFormatKHR> formats) {
        for (auto& availableFormat : formats) {
            if (availableFormat.format == VK_FORMAT_R8G8B8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return availableFormat;
        }

        return formats[0];
    }

    VkPresentModeKHR VulkanSwapchain::choosePresentMode(std::vector<VkPresentModeKHR> presentModes) {
        for (const auto& presentMode : presentModes) {
            if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                LOG_INFO("Swapchain present mode is MAILBOX");
                return presentMode;
            }
        }

        LOG_INFO("Swapchain present mode is FIFO");
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapchain::chooseExtent(VkSurfaceCapabilitiesKHR capabilities) {
        if (capabilities.currentExtent.width != 0xFFFFFFFF) {
            return capabilities.currentExtent;
        }

        int width;
        int height;
        Application::window()->framebufferSize(&width, &height);

        VkExtent2D minExtent = capabilities.minImageExtent;
        VkExtent2D maxExtent = capabilities.maxImageExtent;

        VkExtent2D extent(
            std::clamp(static_cast<uint32_t>(width), minExtent.width, maxExtent.width),
            std::clamp(static_cast<uint32_t>(height), minExtent.height, maxExtent.height)
        );

        return extent;
    }

    void VulkanSwapchain::destroy() {
        if (_swapchain == VK_NULL_HANDLE) {
            return;
        }

        vkDestroySwapchainKHR(_context->device().vkDevice(), _swapchain, nullptr);

        for (auto&& imageView : _imageViews) {
            vkDestroyImageView(_context->device().vkDevice(), imageView, nullptr);
        }

        _swapchain = VK_NULL_HANDLE;
        _context = nullptr;
        // _images.clear();
        // _imageViews.clear();

        LOG_INFO("Swapchain Destroyed!\n");
    }


} // namespace nwt