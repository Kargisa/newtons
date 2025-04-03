#pragma once

#include <vulkan/vulkan.h>

#include "newtons/pch.hpp"
#include "newtons/graphics/graphicsContext.hpp"
#include "vulkanGraphicsPipeline.hpp"
#include "vulkanSwapchain.hpp"
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

    struct VulkanDepthResources {
        VkImage image;
        VkImageView imageView;
        VkDeviceMemory memory;

        VulkanDepthResources() = default;
        VulkanDepthResources(const VkImage& image, const VkImageView& imageView, const VkDeviceMemory& memory)
            : image(image), imageView(imageView), memory(memory) {
        }
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

        static constexpr int MAX_FRAMES_IN_FLIGHT = 3;
        int _currentFrame;

        std::vector<VkFence> _inFlightFences;
        std::vector<VkSemaphore> _imageAvailableSemaphores;
        std::vector<VkSemaphore> _renderFinishedSemaphores;


        VkInstance _instance;
        VkSurfaceKHR _surface;
        const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        VkPhysicalDevice _physicalDevice;
        VkDevice _device;
        VkQueue _graphicsQueue;
        VkQueue _presentQueue;

        VulkanSwapchain _swapChain;
        std::vector<VulkanGraphicsPipeline> _graphicsPipelines;

        VkRenderPass _renderPass;
        VkDescriptorSetLayout _descriptorSetLayout;

        VkCommandPool _graphicsCommandPool;
        std::vector<VkCommandBuffer> _graphicsCommandBuffers;

        VulkanDepthResources _depth;

    protected:
        VulkanContext() = default;

    public:
        virtual ~VulkanContext();

        virtual void init() override;
        virtual void* getNativeContext() override;

        virtual void drawFrame() override;

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
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void recreateSwapChain();

        void createInstance();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSwapChain();
        void createSwapchainImageViews();
        void createRenderPass();
        void createSyncObjects();
        void createDescriptorSetLayout();
        void createDepthResources();
        void createGraphicsCommandPool();
        void createCommandBuffers();
        void createFramebuffers();
        void createGraphicsPipeline(const VulkanShader& shader);

        void cleanupSwapchain();

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    public:
        static GraphicsContext* createContext();
    };
} // namespace nwt