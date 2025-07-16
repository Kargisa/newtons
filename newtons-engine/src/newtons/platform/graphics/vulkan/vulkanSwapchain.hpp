#pragma once

#include <vulkan/vulkan.h>
#include "fixedVector.hpp"
#include "vulkanFramebuffer.hpp"
#include "vulkanSemaphore.hpp"

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
        VkSwapchainKHR _swapchain;
        VkFormat _imageFormat;
        VkExtent2D _extent;
        FixedVector<VkImage> _images;
        FixedVector<VkImageView> _imageViews;
        // FixedVector<VulkanSemaphore> _renderFinishedSemaphores;

    public:
        VulkanSwapchain()
            : _context(nullptr), _swapchain(VK_NULL_HANDLE) {
        }

        VulkanSwapchain(VulkanContext* context)
            : _context(context), _swapchain(VK_NULL_HANDLE) {
        }

        VkSwapchainKHR vkSwapchain() const;
        operator VkSwapchainKHR() const;

        VkFormat vkImageFormat() const;
        const VkExtent2D& vkExtent() const;
        const FixedVector<VkImage>& vkImages() const;
        const FixedVector<VkImageView>& vkImageViews() const;
        // const FixedVector<VulkanSemaphore>& renderFinishedSemaphores() const;

        void initialize(uint32_t width, uint32_t height);
        void destroy();

        void recreate(uint32_t width, uint32_t height);
        VkResult nextImage(const VulkanSemaphore& semaphore, uint32_t* imageIndex) const;
        SupportDetails querySupportDetails();

        VkSurfaceFormatKHR chooseSurfaceFormat(std::vector<VkSurfaceFormatKHR> formats);
        VkPresentModeKHR choosePresentMode(std::vector<VkPresentModeKHR> presentModes);
        VkExtent2D chooseExtent(VkSurfaceCapabilitiesKHR capabilities, uint32_t width, uint32_t height);

    };

} // namespace nwt