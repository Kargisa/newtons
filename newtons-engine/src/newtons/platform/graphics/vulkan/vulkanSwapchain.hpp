#pragma once

#include <vulkan/vulkan.h>

namespace nwt
{
    class VulkanSwapchain {
        VkDevice _device;
        VkSwapchainKHR _vkSwapchain;
        VkExtent2D _extent;
        VkFormat _imageFormat;
        std::vector<VkImage> _images;
        std::vector<VkImageView> _imageViews;
        bool _framebufferResized = false;

        // TODO: create framebuffers etc
    public:
        VulkanSwapchain() {}
        VulkanSwapchain(VkDevice device, VkExtent2D extent, VkFormat imageFormat)
            : _device(device), _extent(extent), _imageFormat(imageFormat) {
        }
        VulkanSwapchain(const VulkanSwapchain& other);
        VulkanSwapchain(VulkanSwapchain&& other) noexcept;


        virtual ~VulkanSwapchain();


        VkSwapchainKHR getVkSwapchain() const;
        VkExtent2D getExtent() const;
        VkFormat getImageFormat() const;
        const std::vector<VkImage>& getImages() const;
        const std::vector<VkImageView>& getImageViews() const;
        bool isFramebufferResized() const;

        VkResult create();

        void destroy();

    private:
        VkResult createFramebuffers();

    };
} // namespace nwt