#pragma once

#include "newtons/pch.hpp"
#include "newtons/renderer/graphicsContext.hpp"
#include <vulkan/vulkan.h>

namespace nwt
{
    struct VulkanQueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        inline bool isComplete() const
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct VulkanSwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct VulkanSwapchainData {
        VkSwapchainKHR swapChain;
        VkExtent2D extent;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        VkFormat imageFormat;
    };

    // *********************************************
    // *************** VulkanContext ***************
    // *********************************************

    class NWT_API VulkanContext : public GraphicsContext
    {

#ifdef DEBUG
        const std::vector<const char*> _validationLayers = {
            "VK_LAYER_KHRONOS_validation" };
        static constexpr bool _enableValidationLayers = true;
#else
        const std::vector<const char*> _validationLayers(0);
        static constexpr bool _enableValidationLayers = false;
#endif

        VkInstance _instance;
        VkSurfaceKHR _surface;
        const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        VkPhysicalDevice _physicalDevice;
        VkDevice _device;
        VkQueue _graphicsQueue;
        VkQueue _presentQueue;

        VulkanSwapchainData _swapchain;

        VkRenderPass _renderPass;
        VkDescriptorSetLayout _descriptorSetLayout;

    protected:
        VulkanContext() = default;

    public:
        virtual ~VulkanContext();

        virtual void init() override;
        virtual void* getNativeContext() override;

    private:
        bool checkValidationLayersSupport();
        bool checkExtensionsSupport(const std::vector<const char*>& requiredExtensions, const std::vector<VkExtensionProperties>& extensions);
        VulkanQueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        VulkanSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        int ratePhysicalDevice(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkFormat findDepthFormat();
        bool hasStencilComponent(VkFormat format);


        void createInstance(const char** thirdPartyExtrensions, size_t thirdPartySize);
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createDescriptorSetLayout();
        void createGraphicsPipeline(const std::vector<char>& vertBinaries, const std::vector<char>& fragBinaries);

    public:
        static GraphicsContext* createContext();
    };
} // namespace nwt