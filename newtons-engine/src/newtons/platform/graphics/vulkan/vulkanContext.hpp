#pragma once

#include <vulkan/vulkan.h>

#include "newtons/pch.hpp"
#include "newtons/graphics/graphicsContext.hpp"
#include "vulkanGraphicsPipeline.hpp"
#include "vulkanSwapchain.hpp"
#include "vulkanDepthBuffer.hpp"
#include "vulkanRenderPass.hpp"
#include "vulkanPhysicalDevice.hpp"
#include "vulkanDevice.hpp"
#include "fixedVector.hpp"

namespace nwt
{
    struct VulkanQueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        inline bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    // *********************************************
    // *************** VulkanContext ***************
    // *********************************************

    class NWT_API VulkanContext : public GraphicsContext
    {
#ifdef DEBUG
        const std::vector<const char*> _validationLayers = { "VK_LAYER_KHRONOS_validation" };
        static constexpr bool _enableValidationLayers = true;
#else
        const std::vector<const char*> _validationLayers(0);
        static constexpr bool _enableValidationLayers = false;
#endif

    public:
        const std::vector<const char*>& validationLayers() const;
        static constexpr bool validationLayersEnabled() {
            return _enableValidationLayers;
        }

    private:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 3;
        int _currentFrame;

        std::vector<VkFence> _inFlightFences;
        std::vector<VkSemaphore> _imageAvailableSemaphores;
        std::vector<VkSemaphore> _renderFinishedSemaphores;


        VkInstance _instance;
        std::vector<VkSurfaceKHR> _surfaces;

        int _selectedPhysicalDeviceIndex = -1;
        FixedVector<VulkanPhysicalDevice> _physicalDevices;
        VulkanDevice _device;

        VulkanSwapchain _swapchain;

        // VulkanRenderPass _renderPass;

        // std::vector<VulkanGraphicsPipeline> _graphicsPipelines;

        // VkDescriptorSetLayout _descriptorSetLayout;

        // VkCommandPool _graphicsCommandPool;
        // std::vector<VkCommandBuffer> _graphicsCommandBuffers;

        // VulkanDepthBuffer _depth;


    public:
        VulkanContext() = default;
        virtual ~VulkanContext();

        virtual void init() override;
        virtual void* getNativeContext() override;

        virtual void drawFrame() override;

        const VulkanDevice& device() const;
        const VulkanPhysicalDevice& physicalDevice() const;
        VkSurfaceKHR vkSurface() const;
        VkInstance vkInstance() const;

        VulkanDepthBuffer* getDepth();

    public:
        bool checkValidationLayersSupport();
        bool checkExtensionsSupport(const std::vector<const char*>& requiredExtensions, const std::vector<VkExtensionProperties>& extensions);
        // VulkanSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

        void createInstance();
        void createSurface();
        void findPhysicalDevices();
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