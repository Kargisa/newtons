
#include "newtons/pch.hpp"
#include "vulkanContext.hpp"
#include "newtons/application.hpp"
#include "vulkanVertex.hpp"
#include "mathf.hpp"
#include "newtons/application.hpp"
#include "vulkanSwapchain.hpp"

#include <thread>
#include <bitset>

namespace nwt
{
    const std::vector<const char*>& VulkanContext::validationLayers() const {
        return _validationLayers;
    }


    VulkanContext::~VulkanContext() {

        LOG_INFO("-------- Cleaning Up --------\n");

        vkDeviceWaitIdle(_device);

        _swapchain.destroy();

        for (auto&& semaphore : _renderFinishedSemaphores) {
            semaphore.destroy();
        }

        for (auto&& framebuffer : _framebuffers) {
            framebuffer.destroy();
        }

        for (auto&& frameInfo : _frameInfos) {
            frameInfo.commandPool.destroy();
            frameInfo.fence.destroy();
            frameInfo.imageAvailabeSemaphore.destroy();

            LOG_INFO("");
        }

        _renderPass.destroy();

        _device.destroy();

        for (size_t i = 0; i < _surfaces.size(); i++) {
            vkDestroySurfaceKHR(_instance, _surfaces[i], nullptr);
        }

        vkDestroyInstance(_instance, nullptr);
    }

    void VulkanContext::initialze() {
        createInstance();
        createSurface();
        findPhysicalDevices();
        pickPhysicalDevice();
        selectQueues();
        createLogicalDevice();
        getQueues();
        createSwapchain();
        createRenderPass();
        createFramebuffers();
        createFrameInfos();


        // createSyncObjects();
        // createDescriptorSetLayout();
        // createDepthResources();
        // createGraphicsCommandPool();
        // createCommandBuffers();
        // createFramebuffers();
        /*
        TODO:
        createShaders();
        createGraphicsPieplines();
        */
    }

    void* VulkanContext::getNativeContext() {
        return this;
    }

    const VulkanDevice& VulkanContext::device() const {
        return _device;
    }

    const VulkanSwapchain& VulkanContext::swapchain() const {
        return _swapchain;
    }

    const FixedVector<VulkanFramebuffer>& VulkanContext::framebuffers() const {
        return _framebuffers;
    }

    const VulkanPhysicalDevice& VulkanContext::physicalDevice() const {
        return _physicalDevices[_selectedPhysicalDeviceIndex];
    }

    const std::vector<VulkanQueue>& VulkanContext::queues() const {
        return _queues;
    }

    const VulkanQueue& VulkanContext::graphicsQueue() const {
        return _queues[0];
    }

    const VulkanQueue& VulkanContext::presentQueue() const {
        return _queues[1];
    }

    const VulkanQueue& VulkanContext::transferQueue() const {
        return _queues[2];
    }

    VkSurfaceKHR VulkanContext::vkSurface() const {
        return _surfaces[0];
    }

    VkInstance VulkanContext::vkInstance() const {
        return _instance;
    }

    VulkanDepthBuffer* VulkanContext::getDepth() const {
        throw std::runtime_error("Depth Not Implemented");
    }

    // ----------- Debugging ----------
    float r = 0.392f;
    float g = 0.584f;
    float b = 0.929f;
    // ------------------------------

    // TODO: Commandbuffers!!! + sync objects etc
    void VulkanContext::drawFrame() {
        static uint64_t i = 0;
        i++;

        const VulkanFrameInfo& frameInfo = _frameInfos[_currentFrame];

        frameInfo.fence.wait(UINT64_MAX);
        frameInfo.fence.reset();

        // LOG_INFO("1: " << i);
        // LOG_INFO("2: " << i);
        uint32_t imageIndex;
        LOG_INFO(i);
        VkResult result = vkAcquireNextImageKHR(_device.vkDevice(), _swapchain.vkSwapchain(), UINT64_MAX, frameInfo.imageAvailabeSemaphore, VK_NULL_HANDLE, &imageIndex);

        const VulkanSemaphore& renderFinishedSemaphore = _renderFinishedSemaphores[imageIndex];

        // if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            //     recreateSwapChain();
            //     return;
            // }
            // else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
                //     throw std::runtime_error("failed to acquire swap chain image!");
                // }


        // //updateUniformBuffer(_currentFrame);

        frameInfo.commandBuffer.reset(0);
        frameInfo.commandBuffer.begin();

        _renderPass.begin(frameInfo, imageIndex);

        // VkViewport viewport{};
        // viewport.x = 0.0f;
        // viewport.y = 0.0f;
        // viewport.width = static_cast<float>(_swapchain.vkExtent().width);
        // viewport.height = static_cast<float>(_swapchain.vkExtent().height);
        // viewport.minDepth = 0.0f;
        // viewport.maxDepth = 1.0f;
        // vkCmdSetViewport(frameInfo.commandBuffer, 0, 1, &viewport);

        // VkRect2D scissor{};
        // scissor.offset = { 0, 0 };
        // scissor.extent = _swapchain.vkExtent();
        // vkCmdSetScissor(frameInfo.commandBuffer, 0, 1, &scissor);

        _renderPass.end(frameInfo);

        frameInfo.commandBuffer.end();


        graphicsQueue().submit({ frameInfo.commandBuffer }, { frameInfo.imageAvailabeSemaphore }, { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT }, { renderFinishedSemaphore }, frameInfo.fence);

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        std::array<VkSemaphore, 1> waitSemaphore = { renderFinishedSemaphore };
        presentInfo.waitSemaphoreCount = waitSemaphore.size();
        presentInfo.pWaitSemaphores = waitSemaphore.data();

        std::array<VkSwapchainKHR, 1> swapChains = std::array<VkSwapchainKHR, 1>();
        swapChains[0] = _swapchain;

        presentInfo.swapchainCount = swapChains.size();
        presentInfo.pSwapchains = swapChains.data();
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        result = vkQueuePresentKHR(presentQueue(), &presentInfo);

        // if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        //     // recreateSwapChain();
        //         // _swapchain.framebufferResized = false;
        // }
        // else if (result != VK_SUCCESS) {
        //     throw std::runtime_error("failed to present swap chain image!");
        // }
        _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
        // vkDeviceWaitIdle(device());
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
                if (layerName[0] != layerProperties.layerName[0]) {
                    continue;
                }

                if (std::strcmp(layerName, layerProperties.layerName)) {
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

        for (const auto& extension : extensions) {
            LOG_INFO("\t" << extension.extensionName);
        }

        LOG_INFO("\n");

        LOG_INFO("Required extensions:");

        bool success = true;

        for (const auto& requiredExtension : requiredExtensions) {
            bool found = false;
            for (const auto& extension : extensions) {
                if (std::strcmp(requiredExtension, extension.extensionName)) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                success = false;
            }

            LOG_INFO("\t" << "Required extension" << RED_COLOR << (found ? " " : " NOT ") << WHITE_COLOR << "found: " << requiredExtension);
        }

        if (success) {
            LOG_INFO("\t" << GREEN_COLOR << "ALL " << WHITE_COLOR << "required extensions found");
        }
        else {
            LOG_INFO("\t" << RED_COLOR << "NOT ALL " << WHITE_COLOR << "required extensions found");
        }

        LOG_INFO("\n");

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
        const char** windowExtensions = Application::window()->getVulkanExtensions(&windowExtensionsCount);

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
        Application::window()->createVulkanSurface(_instance, nullptr, &_surfaces[0]);
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

        if (candidates.rbegin()->first > 0) {
            _selectedPhysicalDeviceIndex = (candidates.rbegin()->second);
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(_physicalDevices[_selectedPhysicalDeviceIndex].vkPhysicalDevice(), &props);
            LOG_INFO("Selected Physical Device: " << props.deviceName << "\n");
        }
        else
            throw std::runtime_error("failed to find a suitable GPU!");
    }

    void VulkanContext::createLogicalDevice() {
        _device = VulkanDevice(this, &_physicalDevices[_selectedPhysicalDeviceIndex]);
        _device.initialize();
        LOG_INFO("Logical Device Created!\n");
    }

    void VulkanContext::selectQueues() {
        if (_queues.size() >= 0) {
            LOG_INFO("selecting queues even though existing!")
        }

        std::vector<VkQueueFamilyProperties> queueProps = physicalDevice().getAvailableQueueFamilyProperties();

        _queues.clear();
        _queues.reserve(4);

        _queues.emplace_back(this, findGraphicsQueueInfo(queueProps));
        _queues.emplace_back(this, findPresentQueueInfo(queueProps));
        _queues.emplace_back(this, findTransferQueueInfo(queueProps));
    }

    void VulkanContext::createSwapchain() {
        _swapchain = VulkanSwapchain(this);
        _swapchain.initialize();

        LOG_INFO("Swapchain Successfully Created!\n");
    }

    void VulkanContext::getQueues() {
        for (auto&& queue : _queues) {
            queue.initialize();
        }
    }

    void VulkanContext::createRenderPass() {
        _renderPass = VulkanRenderPass(this, { r, g, b });
        _renderPass.initialize();
    }

    void VulkanContext::createFramebuffers() {
        const FixedVector<VkImageView>& imageViews = swapchain().vkImageViews();
        _framebuffers = FixedVector<VulkanFramebuffer>(imageViews.size());
        for (size_t i = 0; i < _framebuffers.size(); i++) {
            _framebuffers[i] = VulkanFramebuffer(this);
            _framebuffers[i].initialize(_renderPass, imageViews[i], swapchain().vkExtent());
        }
    }

    void VulkanContext::createFrameInfos() {
        _frameInfos = FixedVector<VulkanFrameInfo>(MAX_FRAMES_IN_FLIGHT);

        _renderFinishedSemaphores = FixedVector<VulkanSemaphore>(swapchain().vkImageViews().size());

        for (auto&& semaphore : _renderFinishedSemaphores) {
            semaphore = VulkanSemaphore(this);
            semaphore.initialize();
        }


        uint32_t i = 0;
        for (auto&& frameInfo : _frameInfos) {
            LOG_INFO("");


            frameInfo.commandPool = VulkanCommandPool(this);
            frameInfo.commandPool.initialize(graphicsQueue().info().family, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

            frameInfo.commandBuffer = VulkanCommandBuffer(this);
            frameInfo.commandBuffer.initialize(frameInfo.commandPool);

            frameInfo.fence = VulkanFence(this);
            frameInfo.fence.initialize(VK_FENCE_CREATE_SIGNALED_BIT);

            frameInfo.imageAvailabeSemaphore = VulkanSemaphore(this);
            frameInfo.imageAvailabeSemaphore.initialize();
            i++;
        }
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
        if (vkCreateImageView(_device.vkDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }

        return imageView;
    }

    VulkanQueueInfo VulkanContext::findPresentQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps) const {
        VulkanQueueInfo queueInfo(-1, 0);

        uint32_t savedFamily = -1;
        uint32_t familyIndex = 0;
        for (auto&& familyProp : availableQueueFamilyProps)
        {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice().vkPhysicalDevice(), familyIndex, vkSurface(), &presentSupport);

            // checks if queue family is able present to the surface
            if (!presentSupport || (familyProp.queueFlags == VK_QUEUE_TRANSFER_BIT) || (familyProp.queueFlags == VK_QUEUE_COMPUTE_BIT)) {
                familyIndex++;
                continue;
            }

            // checks id the current queue family is used by any of the "usedQueues"
            // if not, select current queue family at index 0
            bool familyUsed = false;
            for (auto&& usedQueue : _queues) {
                if (usedQueue.info().family == familyIndex) {
                    familyUsed = true;
                    break;
                }
            }

            if (!familyUsed) {
                queueInfo.family = familyIndex;
                return queueInfo;
            }

            // puts all queue indices of "usedQueues" that have the save queue family as the current queue family in an "unordered_set"
            std::unordered_set<uint32_t> usedQueueIndices;
            for (auto&& usedQueue : _queues) {
                if (usedQueue.info().family == familyIndex) {
                    usedQueueIndices.emplace(usedQueue.info().index);
                }
            }

            // checks for availavle queue indices in the set
            // if an index is free, select current queue family and free queue index
            for (uint32_t i = 0; i < familyProp.queueCount; i++) {
                if (!usedQueueIndices.contains(i)) {
                    queueInfo.family = familyIndex;
                    queueInfo.index = i;
                    return queueInfo;
                }
            }

            // select current queue family as fallback if none was selected
            if (savedFamily == -1) {
                savedFamily = familyIndex;
            }

            familyIndex++;
        }

        // fallback if no unique queue family was found
        if (queueInfo.family == -1) {
            queueInfo.family = savedFamily;
        }

        return queueInfo;
    }
    VulkanQueueInfo VulkanContext::findGraphicsQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps) const {
        VulkanQueueInfo queueInfo(-1, 0);

        uint32_t savedFamily = -1;
        uint32_t familyIndex = 0;
        for (auto&& familyProp : availableQueueFamilyProps)
        {
            VkBool32 presentSupport = false;

            // checks if queue family is able present to the surface
            if (!(familyProp.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                familyIndex++;
                continue;
            }

            // checks id the current queue family is used by any of the "usedQueues"
            // if not, select current queue family at index 0
            bool familyUsed = false;
            for (auto&& usedQueue : _queues) {
                if (usedQueue.info().family == familyIndex) {
                    familyUsed = true;
                    break;
                }
            }

            if (!familyUsed) {
                queueInfo.family = familyIndex;
                return queueInfo;
            }

            // puts all queue indices of "usedQueues" that have the save queue family as the current queue family in an "unordered_set"
            std::unordered_set<uint32_t> usedQueueIndices;
            for (auto&& usedQueue : _queues) {
                if (usedQueue.info().family == familyIndex) {
                    usedQueueIndices.emplace(usedQueue.info().index);
                }
            }

            // checks for availavle queue indices in the set
            // if an index is free, select current queue family and free queue index
            for (uint32_t i = 0; i < familyProp.queueCount; i++) {
                if (!usedQueueIndices.contains(i)) {
                    queueInfo.family = familyIndex;
                    queueInfo.index = i;
                    return queueInfo;
                }
            }

            // select current queue family as fallback if none was selected
            if (savedFamily == -1) {
                savedFamily = familyIndex;
            }
            familyIndex++;
        }

        // fallback if no unique queue family was found
        if (queueInfo.family == -1) {
            queueInfo.family = savedFamily;
        }

        return queueInfo;
    }
    VulkanQueueInfo VulkanContext::findTransferQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps) const {
        VulkanQueueInfo queueInfo(-1, 0);

        uint32_t savedFamily = -1;
        uint32_t familyIndex = 0;

        const uint32_t filter = (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_OPTICAL_FLOW_BIT_NV | VK_QUEUE_VIDEO_DECODE_BIT_KHR | VK_QUEUE_VIDEO_ENCODE_BIT_KHR);

        for (auto&& familyProp : availableQueueFamilyProps) {
            if (!(familyProp.queueFlags & filter) && (familyProp.queueFlags & VK_QUEUE_TRANSFER_BIT)) {
                queueInfo.family = familyIndex;
                return queueInfo;
            }
            familyIndex++;
        }


        familyIndex = 0;
        for (auto&& familyProp : availableQueueFamilyProps) {
            VkBool32 presentSupport = false;

            // checks if queue family is able present to the surface
            if (!(familyProp.queueFlags & (VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT))) {
                familyIndex++;
                continue;
            }

            // checks id the current queue family is used by any of the "usedQueues"
            // if not, select current queue family at index 0
            bool familyUsed = false;
            for (auto&& usedQueue : _queues) {
                if (usedQueue.info().family == familyIndex) {
                    familyUsed = true;
                    break;
                }
            }

            if (!familyUsed) {
                queueInfo.family = familyIndex;
                return queueInfo;
            }

            // puts all queue indices of "usedQueues" that have the save queue family as the current queue family in an "unordered_set"
            std::unordered_set<uint32_t> usedQueueIndices;
            for (auto&& usedQueue : _queues) {
                if (usedQueue.info().family == familyIndex) {
                    usedQueueIndices.emplace(usedQueue.info().index);
                }
            }

            // checks for availavle queue indices in the set
            // if an index is free, select current queue family and free queue index
            for (uint32_t i = 0; i < familyProp.queueCount; i++) {
                if (!usedQueueIndices.contains(i)) {
                    queueInfo.family = familyIndex;
                    queueInfo.index = i;
                    return queueInfo;
                }
            }

            // select current queue family as fallback if none was selected
            if (savedFamily == -1) {
                savedFamily = familyIndex;
            }
            familyIndex++;
        }

        // fallback if no unique queue family was found
        if (queueInfo.family == -1) {
            queueInfo.family = savedFamily;
        }

        return queueInfo;
    }



    GraphicsContext* VulkanContext::create() {
        return new VulkanContext();
    }

} // namespace nwt