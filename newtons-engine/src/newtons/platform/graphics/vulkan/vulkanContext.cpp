
#include "newtons/pch.hpp"
#include "vulkanContext.hpp"
#include "newtons/application.hpp"
#include "vulkanVertex.hpp"
#include "mathf.hpp"
#include "newtons/application.hpp"
#include "vulkanSwapchain.hpp"

namespace nwt
{

    VulkanContext::~VulkanContext() {

        cleanupSwapchain();

        vkDestroyDescriptorSetLayout(_device.getVkDevice(), _descriptorSetLayout, nullptr);

        for (size_t i = 0; i < _graphicsPipelines.size(); i++) {
            _graphicsPipelines[i].cleanup(_device.getVkDevice());
        }


        _renderPass.destroy();
        // vkDestroyRenderPass(_device, _renderPass, nullptr);

        vkDestroyCommandPool(_device.getVkDevice(), _graphicsCommandPool, nullptr);

        vkDestroyDevice(_device.getVkDevice(), nullptr);

        for (size_t i = 0; i < _surfaces.size(); i++) {
            vkDestroySurfaceKHR(_instance, _surfaces[i], nullptr);
        }

        vkDestroyInstance(_instance, nullptr);
    }

    void VulkanContext::init() {
        createInstance();
        createSurface();
        findPhysicalDevices();
        pickPhysicalDevice();

        createLogicalDevice();



        createSwapchain();
        // createSwapchainImageViews();
        createRenderPass();
        createSyncObjects();
        createDescriptorSetLayout();
        createDepthResources();
        createGraphicsCommandPool();
        createCommandBuffers();
        createFramebuffers();
        /*
        TODO:
        createShaders();
        createGraphicsPieplines();
        */
    }

    void* VulkanContext::getNativeContext() {
        return this;
    }

    const VulkanDevice& VulkanContext::getDevice() const {
        return _device;
    }

    VulkanPhysicalDevice VulkanContext::getPhysicalDevice() const {
        return _physicalDevices[_selectedPhysicalDeviceIndex];
    }

    VkSurfaceKHR VulkanContext::getVkSurface() const {
        return _surfaces[0];
    }

    VulkanDepthBuffer* VulkanContext::getDepth() {
        return &_depth;
    }

    void VulkanContext::drawFrame() {
        VkCommandBuffer commandBuffer = _graphicsCommandBuffers[_currentFrame];

        vkWaitForFences(_device.getVkDevice(), 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(_device.getVkDevice(), _swapchain.getVkSwapchain(), UINT64_MAX, _imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        vkResetFences(_device.getVkDevice(), 1, &_inFlightFences[_currentFrame]);

        //updateUniformBuffer(_currentFrame);

        vkResetCommandBuffer(commandBuffer, 0);


        recordCommandBuffer(commandBuffer, imageIndex);


        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { _imageAvailableSemaphores[_currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &_graphicsCommandBuffers[_currentFrame];

        VkSemaphore signalSemaphores[] = { _renderFinishedSemaphores[_currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(_device.getVkGraphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        std::array<VkSwapchainKHR, 1> swapChains = std::array<VkSwapchainKHR, 1>();
        swapChains[0] = _swapchain.getVkSwapchain();

        presentInfo.swapchainCount = swapChains.size();
        presentInfo.pSwapchains = swapChains.data();
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        result = vkQueuePresentKHR(_device.getVkPresentQueue(), &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _swapchain.isFramebufferResized()) {
            recreateSwapChain();
            // _swapchain.framebufferResized = false;
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    // *********************************************
    // ************** Private Members **************
    // *********************************************

    bool VulkanContext::checkValidationLayersSupport()
    {
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : _validationLayers)
        {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers)
            {
                if (std::strcmp(layerName, layerProperties.layerName))
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
                return false;
        }

        return true;
    }

    bool VulkanContext::checkExtensionsSupport(const std::vector<const char*>& requiredExtensions, const std::vector<VkExtensionProperties>& extensions)
    {
        LOG_INFO("Extensions:");

        for (const auto& extension : extensions)
            LOG_INFO("\t" << extension.extensionName);

        LOG_INFO("\n");

        LOG_INFO("Required extensions:");

        uint16_t foundExtensionsCount = 0;

        bool success = true;

        for (const auto& requiredExtension : requiredExtensions)
        {
            bool found = false;
            for (const auto& extension : extensions)
            {
                if (std::strcmp(requiredExtension, extension.extensionName))
                {
                    found = true;
                    foundExtensionsCount++;
                    break;
                }
            }

            if (!found)
                success = false;

            LOG_INFO("\t" << "Required extension" << RED_COLOR << (found ? " " : " NOT ") << WHITE_COLOR << "found: " << requiredExtension);
        }

        if (foundExtensionsCount == requiredExtensions.size())
        {
            LOG_INFO("\t" << GREEN_COLOR << "ALL " << WHITE_COLOR << "required extensions found");
        }
        else
        {
            LOG_INFO("\t" << RED_COLOR << "NOT ALL " << WHITE_COLOR << "required extensions found");
        }

        return success;
    }

    void VulkanContext::findPhysicalDevices() {
        uint32_t count = 0;
        vkEnumeratePhysicalDevices(_instance, &count, nullptr);

        _physicalDevices = FixedVector<VulkanPhysicalDevice>(static_cast<size_t>(count));
        std::vector<VkPhysicalDevice> devices(count);
        vkEnumeratePhysicalDevices(_instance, &count, devices.data());

        for (size_t i = 0; i < count; i++) {
            _physicalDevices[i] = VulkanPhysicalDevice(this, devices[i]);
        }
    }

    void VulkanContext::createInstance() {
        if (_enableValidationLayers && !checkValidationLayersSupport())
            throw std::runtime_error("Validation layers requested, but not available!");

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "NEWTONS";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        if constexpr (_enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
            createInfo.ppEnabledLayerNames = _validationLayers.data();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        uint32_t windowExtensionsCount = 0;
        const char** windowExtensions = Application::getWindow()->getVulkanExtensions(&windowExtensionsCount);

        std::vector<const char*> requiredExtensions(windowExtensionsCount);

        for (uint32_t i = 0; i < windowExtensionsCount; i++) {
            requiredExtensions[i] = windowExtensions[i];
        }

        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

        createInfo.enabledExtensionCount = requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        // createInfo.enabledLayerCount = 0;

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        if (!checkExtensionsSupport(requiredExtensions, extensions))
            throw std::runtime_error("Required extensions not found!");

        VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);

        if (result != VK_SUCCESS)
            throw std::runtime_error("failed to create instance!");
    }

    void VulkanContext::createSurface() {
        _surfaces.resize(1, nullptr);
        Application::getWindow()->createVulkanSurface(_instance, nullptr, &_surfaces[0]);
    }

    void VulkanContext::pickPhysicalDevice() {

        if (_physicalDevices.size() == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::multimap<uint32_t, size_t> candidates;

        size_t index = 0;
        for (auto&& device : _physicalDevices) {
            uint32_t score = device.getRating();
            candidates.insert(std::make_pair(score, index));
            index++;
        }

        if (candidates.rbegin()->first > 0)
            _selectedPhysicalDeviceIndex = candidates.rbegin()->second;
        else
            throw std::runtime_error("failed to find a suitable GPU!");
    }

    void VulkanContext::createLogicalDevice() {
        _device = VulkanDevice(this, &_physicalDevices[_selectedPhysicalDeviceIndex]);
    }

    VkSurfaceFormatKHR VulkanContext::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_R8G8B8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return availableFormat;
        }

        return availableFormats[0];
    }

    VkPresentModeKHR VulkanContext::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        LOG_INFO("Present mode is FIFO");
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanContext::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }

        int width = 0, height = 0;

        Application::getWindow()->getFramebufferSize(&width, &height);

        VkExtent2D extent(
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height));

        VkExtent2D minExtent = capabilities.minImageExtent;
        VkExtent2D maxExtent = capabilities.maxImageExtent;

        extent.width = Mathf::clamp(extent.width, minExtent.width, maxExtent.width);
        extent.height = Mathf::clamp(extent.height, minExtent.height, maxExtent.height);

        return extent;
    }

    void VulkanContext::createSwapchain() {

    }

    VkImageView VulkanContext::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(_device.getVkDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }

        return imageView;
    }



    // VkFormat VulkanContext::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    //     for (VkFormat format : candidates) {
    //         VkFormatProperties props;
    //         vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &props);

    //         if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
    //             return format;
    //         }
    //         else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
    //             return format;
    //         }
    //     }

    //     throw std::runtime_error("failed to find supported format!");
    // }

    // VkFormat VulkanContext::findDepthFormat() {
    //     return findSupportedFormat(
    //         { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
    //         VK_IMAGE_TILING_OPTIMAL,
    //         VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    //     );
    // }

    // bool VulkanContext::hasStencilComponent(VkFormat format)
    // {
    //     return (
    //         format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
    //         format == VK_FORMAT_D24_UNORM_S8_UINT ||
    //         format == VK_FORMAT_D16_UNORM_S8_UINT
    //         );
    // }

    // void VulkanContext::createRenderPass() {
    //     VkRect2D renderArea;
    //     renderArea.offset = { 0 ,0 };
    //     renderArea.extent = _swapchain.getExtent();

    //     VkClearColorValue clearColor = { 0.08f, 0.08f, 0.1f, 1.0f };
    //     VkClearDepthStencilValue clearDepthStencil = { 1, 0 };

    //     _renderPass = VulkanRenderPass(_device, clearColor, clearDepthStencil, renderArea);
    //     _renderPass.create(_swapchain.getImageFormat(), findDepthFormat());
    // }

    // void VulkanContext::createSyncObjects() {
    //     _imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    //     _renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    //     _inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    //     VkSemaphoreCreateInfo semaphoreInfo{};
    //     semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    //     VkFenceCreateInfo fenceInfo{};
    //     fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    //     fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    //     for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    //         if (vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
    //             vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
    //             vkCreateFence(_device, &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS) {
    //             throw std::runtime_error("failed to create synchronization objects for a frame!");
    //         }
    //     }
    // }

    // void VulkanContext::createDescriptorSetLayout() {
    //     VkDescriptorSetLayoutBinding uboLayoutBinding{};
    //     uboLayoutBinding.binding = 0;
    //     uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    //     uboLayoutBinding.descriptorCount = 1;
    //     uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    //     uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

    //     VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    //     samplerLayoutBinding.binding = 1;
    //     samplerLayoutBinding.descriptorCount = 1;
    //     samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //     samplerLayoutBinding.pImmutableSamplers = nullptr;
    //     samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    //     std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
    //     VkDescriptorSetLayoutCreateInfo layoutInfo{};
    //     layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    //     layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    //     layoutInfo.pBindings = bindings.data();

    //     if (vkCreateDescriptorSetLayout(_device, &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS) {
    //         throw std::runtime_error("failed to create descriptor set layout!");
    //     }
    // }

    // uint32_t VulkanContext::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    //     VkPhysicalDeviceMemoryProperties memProperties;
    //     vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

    //     for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    //         if (typeFilter & (1u << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
    //             return i;
    //         }
    //     }

    //     throw std::runtime_error("failed to find suitable memory type!");
    // }

    // void VulkanContext::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
    //     VkImageCreateInfo imageInfo{};
    //     imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    //     imageInfo.imageType = VK_IMAGE_TYPE_2D;
    //     imageInfo.extent.width = static_cast<uint32_t>(width);
    //     imageInfo.extent.height = static_cast<uint32_t>(height);
    //     imageInfo.extent.depth = 1;
    //     imageInfo.mipLevels = 1;
    //     imageInfo.arrayLayers = 1;
    //     imageInfo.format = format;
    //     imageInfo.tiling = tiling;
    //     imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //     imageInfo.usage = usage;
    //     imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    //     imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    //     imageInfo.flags = 0; // Optional

    //     if (vkCreateImage(_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
    //         throw std::runtime_error("failed to create image!");
    //     }

    //     VkMemoryRequirements memRequirements;
    //     vkGetImageMemoryRequirements(_device, image, &memRequirements);

    //     VkMemoryAllocateInfo allocInfo{};
    //     allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    //     allocInfo.allocationSize = memRequirements.size;
    //     allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    //     if (vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
    //         throw std::runtime_error("failed to allocate image memory!");
    //     }

    //     vkBindImageMemory(_device, image, imageMemory, 0);
    // }

    // void VulkanContext::recreateSwapChain() {
    //     int width = 0, height = 0;
    //     Application::getWindow()->getFramebufferSize(&width, &height);

    //     vkDeviceWaitIdle(_device); //TODO: better way to do this than idle

    //     cleanupSwapchain();

    //     createSwapchain();
    //     // createSwapchainImageViews();
    //     createDepthResources();
    //     createFramebuffers();
    // }

    // void VulkanContext::createDepthResources() {
    //     VkFormat depthFormat = findDepthFormat();
    //     createImage(_swapchain.getExtent().width, _swapchain.getExtent().height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _depth.image, _depth.memory);
    //     _depth.imageView = createImageView(_depth.image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    // }

    // void VulkanContext::createGraphicsCommandPool() {
    //     VulkanQueueFamilyIndices queueFamilyIndices = findQueueFamilies(_physicalDevice);

    //     VkCommandPoolCreateInfo poolInfo{};
    //     poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    //     poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    //     poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    //     if (vkCreateCommandPool(_device, &poolInfo, nullptr, &_graphicsCommandPool) != VK_SUCCESS) {
    //         throw std::runtime_error("failed to create command pool!");
    //     }
    // }

    // void VulkanContext::createCommandBuffers() {
    //     _graphicsCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    //     VkCommandBufferAllocateInfo allocInfo{};
    //     allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    //     allocInfo.commandPool = _graphicsCommandPool;
    //     allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    //     allocInfo.commandBufferCount = static_cast<uint32_t>(_graphicsCommandBuffers.size());

    //     if (vkAllocateCommandBuffers(_device, &allocInfo, _graphicsCommandBuffers.data()) != VK_SUCCESS) {
    //         throw std::runtime_error("failed to allocate command buffers!");
    //     }
    // }

    // void VulkanContext::createFramebuffers() {
    //     _renderPass.getFramebuffers().resize(_swapchain.getImageViews().size());

    //     for (size_t i = 0; i < _swapchain.getImageViews().size(); i++) {
    //         std::array<VkImageView, 2> attachments = {
    //             _swapchain.getImageViews()[i],
    //             _depth.imageView
    //         };

    //         VkFramebufferCreateInfo framebufferInfo{};
    //         framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    //         framebufferInfo.renderPass = _renderPass.getVkRenderPass();
    //         framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    //         framebufferInfo.pAttachments = attachments.data();
    //         framebufferInfo.width = _swapchain.getExtent().width;
    //         framebufferInfo.height = _swapchain.getExtent().height;
    //         framebufferInfo.layers = 1;

    //         if (vkCreateFramebuffer(_device, &framebufferInfo, nullptr, &_renderPass.getFramebuffers()[i]) != VK_SUCCESS) {
    //             throw std::runtime_error("failed to create framebuffer!");
    //         }
    //     }
    // }

    // void VulkanContext::cleanupSwapchain() {
    //     vkDestroyImageView(_device, _depth.imageView, nullptr);
    //     vkDestroyImage(_device, _depth.image, nullptr);
    //     vkFreeMemory(_device, _depth.memory, nullptr);

    //     for (auto framebuffer : _renderPass.getFramebuffers()) {
    //         vkDestroyFramebuffer(_device, framebuffer, nullptr);
    //     }

    //     for (auto imageView : _swapchain.getImageViews()) {
    //         vkDestroyImageView(_device, imageView, nullptr);
    //     }

    //     vkDestroySwapchainKHR(_device, _swapchain.getVkSwapchain(), nullptr);
    // }

    // void VulkanContext::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    //     VkCommandBufferBeginInfo beginInfo{};
    //     beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    //     beginInfo.flags = 0; // Optional
    //     beginInfo.pInheritanceInfo = nullptr; // Optional

    //     if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    //         throw std::runtime_error("failed to begin recording command buffer!");
    //     }

    //     _renderPass.begin(commandBuffer, _swapchain.framebuffers[imageIndex]);

    //     //vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);

    //     //VkBuffer vertexBuffers[] = { _vertexBuffer };
    //     //VkDeviceSize offsets[] = { 0 };
    //     //vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    //     //vkCmdBindIndexBuffer(commandBuffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    //     VkViewport viewport{};
    //     viewport.x = 0.0f;
    //     viewport.y = 0.0f;
    //     viewport.width = static_cast<float>(_swapchain.getExtent().width);
    //     viewport.height = static_cast<float>(_swapchain.getExtent().height);
    //     viewport.minDepth = 0.0f;
    //     viewport.maxDepth = 1.0f;
    //     vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    //     VkRect2D scissor{};
    //     scissor.offset = { 0, 0 };
    //     scissor.extent = _swapchain.getExtent();
    //     vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    //     //vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &_descriptorSets[_currentFrame], 0, nullptr);

    //     //vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    //     // ********************************
    //     // draw commands here *************
    //     // ********************************

    //     _renderPass.end(commandBuffer);
    //     if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    //         throw std::runtime_error("failed to record command buffer!");
    //     }
    // }

    GraphicsContext* VulkanContext::create() {
        return new VulkanContext();
    }

} // namespace nwt