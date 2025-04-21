#pragma once

#include <vulkan/vulkan.h>

#include "newtons/pch.hpp"
#include "newtons/graphics/graphicsContext.hpp"
#include "vulkanGraphicsPipeline.hpp"
#include "vulkanSwapchain.hpp"
#include "vulkanDepthBuffer.hpp"
#include "vulkanRenderPass.hpp"

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
        std::vector<VkSurfaceKHR> _surfaces;
        const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        VkPhysicalDevice _physicalDevice;
        VkDevice _device;
        VkQueue _graphicsQueue;
        VkQueue _presentQueue;

        VulkanSwapchain _swapchain;
        std::vector<VulkanGraphicsPipeline> _graphicsPipelines;

        VulkanRenderPass _renderPass;
        VkDescriptorSetLayout _descriptorSetLayout;

        VkCommandPool _graphicsCommandPool;
        std::vector<VkCommandBuffer> _graphicsCommandBuffers;

        VulkanDepthBuffer _depth;

    protected:
        VulkanContext() = default;

    public:
        virtual ~VulkanContext();

        virtual void init() override;
        virtual void* getNativeContext() override;

        virtual void drawFrame() override;

        VkDevice getDevice() const;
        VkPhysicalDevice getPhysicalDevice() const;
        VkSurfaceKHR getSurface() const;

        VulkanDepthBuffer* getDepth();

    public:
        bool checkValidationLayersSupport();
        bool checkExtensionsSupport(const std::vector<const char*>& requiredExtensions, const std::vector<VkExtensionProperties>& extensions);
        VulkanQueueFamilyIndices findQueueFamilies();
        VulkanQueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        // VulkanSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
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
        void createSwapchain();
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
        static GraphicsContext* create();
    };
} // namespace nwt