#include "vulkanSwapchain.hpp"



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

    void VulkanSwapchain::destroy() {
    }


} // namespace nwt