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
#include "vulkanFrameInfo.hpp"
#include "vulkanCommandPool.hpp"
#include "vulkanCommandBuffer.hpp"
#include "vulkanFramebuffer.hpp"
#include "vulkanSemaphore.hpp"
#include "vulkanFence.hpp"
#include "vulkanQueue.hpp"

namespace nwt
{




    // *********************************************
    // *************** VulkanContext ***************
    // *********************************************

    class NWT_API VulkanContext : public GraphicsContext
    {
#ifdef NWT_DEBUG
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
        int _currentFrame = 0;

        VkInstance _instance;
        std::vector<VkSurfaceKHR> _surfaces;

        int _selectedPhysicalDeviceIndex = -1;
        FixedVector<VulkanPhysicalDevice> _physicalDevices;
        VulkanDevice _device;

        /*
        Reserved Queue Indices:
        [0]: Graphics
        [1]: Present
        [2]: Transfer
        [3]: Compute
        */
        std::vector<VulkanQueue> _queues;

        VulkanSwapchain _swapchain;
        VulkanRenderPass _renderPass;

        FixedVector<VulkanFrameInfo> _frameInfos;

    public:

        // std::vector<VulkanGraphicsPipeline> _graphicsPipelines;

        // VkDescriptorSetLayout _descriptorSetLayout;


        // VulkanDepthBuffer _depth;


    public:
        VulkanContext() = default;
        virtual ~VulkanContext();

        virtual void                                initialize() override;
        virtual void* getNativeContext() override;

        virtual void                                drawFrame() override;

        VkInstance                                  vkInstance() const;
        VkSurfaceKHR                                vkSurface() const;
        const VulkanPhysicalDevice& physicalDevice() const;

        const std::vector<VulkanQueue>& queues() const;
        const VulkanQueue& graphicsQueue() const;
        const VulkanQueue& presentQueue() const;
        const VulkanQueue& transferQueue() const;

        const VulkanDevice& device() const;
        const VulkanSwapchain& swapchain() const;

        VulkanDepthBuffer* getDepth() const;

    public:
        bool                                        checkValidationLayersSupport();
        bool                                        checkExtensionsSupport(const std::vector<const char*>& requiredExtensions, const std::vector<VkExtensionProperties>& extensions);
        VkImageView                                 createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

        void                                        createInstance();
        void                                        createSurface();
        void                                        findPhysicalDevices();
        void                                        pickPhysicalDevice();
        void                                        createLogicalDevice();
        void                                        selectQueues();
        void                                        createSwapchain();
        void                                        getQueues();
        void                                        createRenderPass();
        void                                        createFrameInfos();


        VkResult present(uint32_t imageIndex, const VulkanSemaphore& semaphore);
        void createDescriptorSetLayout();
        void createDepthResources();
        void createCommandBuffers();
        void createGraphicsPipeline(const VulkanShader& shader);

        VulkanQueueInfo                             findPresentQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps) const;
        VulkanQueueInfo                             findGraphicsQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps) const;
        VulkanQueueInfo                             findTransferQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps) const;


    public:
        static GraphicsContext* create();
    };
} // namespace nwt