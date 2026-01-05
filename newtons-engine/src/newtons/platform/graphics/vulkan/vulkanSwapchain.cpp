#include "vulkanSwapchain.hpp"
#include "vulkanContext.hpp"



namespace nwt
{
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

    // const FixedVector<VulkanSemaphore>& VulkanSwapchain::renderFinishedSemaphores() const {
    //     return _renderFinishedSemaphores;
    // }

    void VulkanSwapchain::initialize(uint32_t width, uint32_t height) {
        const VulkanDevice& device = _context->device();
        SupportDetails swapChainDetails = querySupportDetails();


        VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(swapChainDetails.formats);
        VkPresentModeKHR presentMode = choosePresentMode(swapChainDetails.presentModes);
        VkExtent2D extent = chooseExtent(swapChainDetails.capabilities, width, height);

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

            std::array<uint32_t, 2> queueFamilyIndices = { graphicsQueueInfo.family, presentQueueInfo.family };
            createInfo.queueFamilyIndexCount = queueFamilyIndices.size();
            createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
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
        // _renderFinishedSemaphores = FixedVector<VulkanSemaphore>(_images.size());
        for (size_t i = 0; i < _images.size(); i++) {
            _imageViews[i] = _context->createImageView(_images[i], _imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
            // _renderFinishedSemaphores[i] = VulkanSemaphore(_context);
            // _renderFinishedSemaphores[i].initialize();
        }
    }

    // INFO: Heavy performance tank under repeated swapchain recreation 
    VulkanSwapchain::SupportDetails VulkanSwapchain::querySupportDetails() {
        VkSurfaceKHR surface = _context->vkSurface();
        const VkPhysicalDevice& phDevice = _context->physicalDevice();

        VulkanSwapchain::SupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phDevice, surface, &details.capabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(phDevice, surface, &formatCount, nullptr); // very expensive should be cached

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
        for (auto availableFormat : formats) {
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

    VkExtent2D VulkanSwapchain::chooseExtent(VkSurfaceCapabilitiesKHR capabilities, uint32_t width, uint32_t height) {
        if (capabilities.currentExtent.width != 0xFFFFFFFF) {
            return capabilities.currentExtent;
        }

        VkExtent2D minExtent = capabilities.minImageExtent;
        VkExtent2D maxExtent = capabilities.maxImageExtent;

        VkExtent2D extent(
            std::clamp(width, minExtent.width, maxExtent.width),
            std::clamp(height, minExtent.height, maxExtent.height)
        );

        return extent;
    }

    void VulkanSwapchain::destroy() {
        if (_swapchain == VK_NULL_HANDLE) {
            return;
        }

        vkDestroySwapchainKHR(_context->device().vkDevice(), _swapchain, nullptr);

        for (size_t i = 0; i < _imageViews.size(); i++) {
            vkDestroyImageView(_context->device().vkDevice(), _imageViews[i], nullptr);
            // _renderFinishedSemaphores[i].destroy();
        }

        _swapchain = VK_NULL_HANDLE;
        _context = nullptr;
        // _images.clear();
        // _imageViews.clear();

        LOG_INFO("Swapchain Destroyed!");
    }

    void VulkanSwapchain::recreate(uint32_t windowWidth, uint32_t windowHeight) {
        destroy();
        initialize(windowWidth, windowHeight);
    }

    VkResult VulkanSwapchain::nextImage(const VulkanSemaphore& semaphore, uint32_t* imageIndex) const {
        return vkAcquireNextImageKHR(_context->device(), _swapchain, UINT64_MAX, semaphore, VK_NULL_HANDLE, imageIndex);
    }


} // namespace nwt