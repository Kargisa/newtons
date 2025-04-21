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


        VulkanContext* _context;
        bool _framebufferResized = false;

        VkSwapchainKHR _vkSwapchain;
        VkExtent2D _extent;
        VkFormat _imageFormat;
        std::vector<VkImage> _images;
        std::vector<VkImageView> _imageViews;

        // TODO: create framebuffers etc
    public:
        VulkanSwapchain() {}
        VulkanSwapchain(VulkanContext* context)
            : _context(context) {
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
        SupportDetails querySupportDetails();
        static SupportDetails querySupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface);
        // TODO:
        VkSurfaceFormatKHR chooseSurfaceFormat(std::vector<VkSurfaceFormatKHR> formats);
        VkPresentModeKHR choosePresentMode(std::vector<VkPresentModeKHR> presentModes);
        VkExtent2D chooseExtent(VkSurfaceCapabilitiesKHR capabilities);

        void createImageViews();
        // END TODO
        void destroy();

    private:
        VkResult createFramebuffers();
    };

} // namespace nwt