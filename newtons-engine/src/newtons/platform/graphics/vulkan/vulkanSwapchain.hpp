#pragma once

#include <vulkan/vulkan.h>

namespace nwt
{

    struct VulkanContext;

    class VulkanSwapchain {
    public:
        struct SupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };


    private:
        VulkanContext* _context;
        VkSwapchainKHR _vkSwapchain;
        VkFormat _imageFormat;
        VkExtent2D _extent;
        std::vector<VkImage> _images;
        std::vector<VkImageView> _imageViews;

        bool _framebufferResized;

    public:
        VulkanSwapchain()
            : _context(nullptr), _vkSwapchain(nullptr), _framebufferResized(false) {
        }

        VulkanSwapchain(VulkanContext* context)
            : _context(context), _vkSwapchain(nullptr), _framebufferResized(false) {
        }
        // ~VulkanSwapchain();

        VulkanSwapchain(const VulkanSwapchain& other) = delete;

        VulkanSwapchain& operator=(const VulkanSwapchain& other);

        VkSwapchainKHR vkSwapchain() const;
        VkFormat vkImageFormat() const;
        const VkExtent2D& vkExtent() const;
        const std::vector<VkImage>& vkImages() const;
        const std::vector<VkImageView>& vkImageViews() const;
        bool isFramebufferResized() const;

        void initialize();
        SupportDetails querySupportDetails();

        VkSurfaceFormatKHR chooseSurfaceFormat(std::vector<VkSurfaceFormatKHR> formats);
        VkPresentModeKHR choosePresentMode(std::vector<VkPresentModeKHR> presentModes);
        VkExtent2D chooseExtent(VkSurfaceCapabilitiesKHR capabilities);

        void destroy();
    };

} // namespace nwt