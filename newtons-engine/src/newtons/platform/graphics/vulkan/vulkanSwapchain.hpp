#pragma once

#include <vulkan/vulkan.h>

namespace nwt
{
    struct VulkanSwapchain {
        VkSwapchainKHR swapChain;
        VkExtent2D extent;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        VkFormat imageFormat;
        std::vector<VkFramebuffer> framebuffers;
        bool framebufferResized = false;
    };
} // namespace nwt