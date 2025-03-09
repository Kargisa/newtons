#pragma once

#include "newtons/pch.hpp"
#include "newtons/graphics/graphicsContext.hpp"
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
        std::vector<VkFramebuffer> framebuffers;
        bool framebufferResized = false;
    };

    struct VulkanGraphicsPipelineData {
        VkPipeline pipeline;
        VkPipelineLayout layout;

        VulkanGraphicsPipelineData() = default;
        VulkanGraphicsPipelineData(const VkPipeline& pipeline, const VkPipelineLayout& layout)
            : pipeline(pipeline), layout(layout) {}
    };

    struct VulkanDepthResources {
        VkImage image;
        VkImageView imageView;
        VkDeviceMemory memory;

        VulkanDepthResources() = default;
        VulkanDepthResources(const VkImage& image, const VkImageView& imageView, const VkDeviceMemory& memory)
            : image(image), imageView(imageView), memory(memory) {}
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

        VulkanSwapchainData _swapChain;
        std::unordered_map<uint32_t, VulkanGraphicsPipelineData> _graphicsPipelines;

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
        VkShaderModule createShaderModule(uint32_t* code, uint32_t size) const;
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void createGraphicsPipeline(uint32_t* vertBinaries, uint32_t vSize, const std::string& vEntry, uint32_t* fragBinaries, uint32_t fSize, const std::string& fEntry, uint32_t id);
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

        void cleanupSwapchain();

        void recordCommandBuffer(VkCommandBuffer commandBuffer);

    public:
        static GraphicsContext* createContext();
    };
} // namespace nwt