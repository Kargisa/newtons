
#include "newtons/pch.hpp"
#include "vulkanContext.hpp"
#include "newtons/application.hpp"
#include "vulkanVertex.hpp"
#include "mathf.hpp"
#include "newtons/application.hpp"
#include "vulkanSwapchain.hpp"
#include "newtons/io/file.hpp"

#include <thread>
#include <bitset>

#include "vulkanReflect.hpp"

#define VK_VERSION VK_API_VERSION_1_3

namespace nwt
{
    const std::vector<const char*>& VulkanContext::validationLayers() const {
        return _validationLayers;
    }


    VulkanContext::~VulkanContext() {
        LOG_INFO("-------- Cleaning Up --------\n");

        device().waitIdle();

        vertexBuffer.destroy();
        indexBuffer.destroy();
        vmaDestroyAllocator(_allocator);

        LOG_SPACE();

        _trianglePipeline.destroy();

        _swapchain.destroy();

        LOG_SPACE();

        for (auto&& framebuffer : _framebuffers) {
            framebuffer.destroy();
        }

        LOG_SPACE();

        for (auto&& semaphore : _renderFinishedSemaphore) {
            semaphore.destroy();
        }

        LOG_SPACE();

        for (auto&& semaphore : _imageAvailableSemaphores) {
            semaphore.destroy();
        }

        LOG_SPACE();

        for (auto&& fence : _renderFinishedFences) {
            fence.destroy();
        }

        LOG_SPACE();

        for (auto&& fence : _transferFinishedFences) {
            fence.destroy();
        }

        LOG_SPACE();

        for (auto&& pool : _graphicsCommandPools) {
            pool.destroy();
        }

        LOG_SPACE();

        for (auto&& pool : _copyCommandPools) {
            pool.destroy();
        }

        LOG_SPACE();

        _renderPass.destroy();

        _device.destroy();

        for (size_t i = 0; i < _surfaces.size(); i++) {
            vkDestroySurfaceKHR(_instance, _surfaces[i], nullptr);
        }

        vkDestroyInstance(_instance, nullptr);
    }

    void VulkanContext::initialize() {
        createInstance();
        createSurface();
        findPhysicalDevices();
        pickPhysicalDevice();
        selectQueues();
        createLogicalDevice();
        initializeQueues();
        createVmaAllocator();
        createSwapchain();
        createRenderPass();
        createFramebuffers();
        createCommandObjects();
        createSyncObjects();

        //INFO: Debug:
        FixedVector<char> vertSpirV = FileReader::readSpirV("../../../../assets/shaders/compiledShaders/triangleVert.spv");
        FixedVector<char> fragSpirV = FileReader::readSpirV("../../../../assets/shaders/compiledShaders/triangleFrag.spv");

        _trianglePipeline = VulkanGraphicsPipeline(this);
        _trianglePipeline.initialize(reinterpret_cast<uint32_t*>(vertSpirV.data()), vertSpirV.size(), reinterpret_cast<uint32_t*>(fragSpirV.data()), fragSpirV.size());

        VulkanBuffer v_stagingBuffer = VulkanBuffer(this, sizeof(Vec3) * 3);
        v_stagingBuffer.initialize(VulkanBufferType::STAGING_BUFFER);

        std::array<Vec3, 3> vertices;
        vertices[0] = Vec3{ 0.5, 0.5, 0 };
        vertices[1] = Vec3{ -0.5, 0.5, 0 };
        vertices[2] = Vec3{ 0.0, -0.5, 0 };


        void* v_data;
        v_stagingBuffer.mapMemory(&v_data);
        memcpy(v_data, &vertices, sizeof(Vec3) * vertices.size());
        v_stagingBuffer.unmapMemory();

        vertexBuffer = VulkanBuffer(this, sizeof(Vec3) * vertices.size());
        vertexBuffer.initialize(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_AUTO, 0);

        copyBuffer(v_stagingBuffer, vertexBuffer, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);


        std::array<uint32_t, 3> indices = { 0, 1, 2 };

        VulkanBuffer i_stagingBuffer = VulkanBuffer(this, sizeof(uint32_t) * indices.size());
        i_stagingBuffer.initialize(VulkanBufferType::STAGING_BUFFER);

        void* i_data;
        i_stagingBuffer.mapMemory(&i_data);
        memcpy(i_data, &indices, sizeof(uint32_t) * indices.size());
        i_stagingBuffer.unmapMemory();

        indexBuffer = VulkanBuffer(this, sizeof(uint32_t) * indices.size());
        indexBuffer.initialize(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VMA_MEMORY_USAGE_AUTO, 0);

        copyBuffer(i_stagingBuffer, indexBuffer, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, VK_ACCESS_INDEX_READ_BIT);

        VulkanReflect::spirvReflectExample(reinterpret_cast<uint32_t*>(fragSpirV.data()), fragSpirV.size());

        // createDescriptorSetLayout();
        // createDepthResources();
        // createGraphicsCommandPool();
        // createCommandBuffers();
    }

    void* VulkanContext::nativeContext() {
        return this;
    }

    GraphicsAPI VulkanContext::getAPI() const {
        return GraphicsAPI::VULKAN;
    }

    const VulkanDevice& VulkanContext::device() const {
        return _device;
    }

    const VulkanSwapchain& VulkanContext::swapchain() const {
        return _swapchain;
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
        LOG_FAIL("Depth Not Implemented");
        throw std::runtime_error("");
    }

    const VulkanRenderPass& VulkanContext::renderPass() const {
        return _renderPass;
    }

    VmaAllocator VulkanContext::vmaAllocator() const {
        return _allocator;
    }

    // ----------- Debugging ----------
    float r = 0.392f;
    float g = 0.584f;
    float b = 0.929f;
    // ------------------------------

    void VulkanContext::drawFrame() {
        const VulkanFence& transferFinishedFence = _transferFinishedFences[_currentFrame];
        const VulkanCommandBuffer& transferCommandbuffer = _transferCommandBuffers[_currentFrame];

        transferFinishedFence.reset();

        transferCommandbuffer.reset();
        transferCommandbuffer.begin();

        for (auto&& info : _bufferCopyInfos) {
            VkBufferCopy region = {};
            region.dstOffset = 0;
            region.srcOffset = 0;
            region.size = info.srcBuffer.size();

            vkCmdCopyBuffer(transferCommandbuffer, info.srcBuffer, info.dstBuffer, 1, &region);
        }

        FixedVector<VkBufferMemoryBarrier> barriers(_bufferCopyInfos.size());

        int i = 0;
        for (auto&& info : _bufferCopyInfos) {
            VkBufferMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            barrier.buffer = info.dstBuffer;
            barrier.offset = 0;
            barrier.size = VK_WHOLE_SIZE;
            barrier.srcQueueFamilyIndex = transferQueue().info().family;
            barrier.dstQueueFamilyIndex = graphicsQueue().info().family;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = 0;

            barriers[i++] = barrier;
        }

        vkCmdPipelineBarrier(
            transferCommandbuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            0,
            0, VK_NULL_HANDLE,
            barriers.size(), barriers.data(),
            0, VK_NULL_HANDLE
        );


        transferCommandbuffer.end();
        transferQueue().submit({ transferCommandbuffer }, {}, {}, {}, { transferFinishedFence });
        transferFinishedFence.wait();
        _bufferCopyInfos.clear();

        const VulkanSemaphore& imageAvailabeSemaphore = _imageAvailableSemaphores[_currentFrame];
        const VulkanFence& renderFinishedFence = _renderFinishedFences[_currentFrame];
        const VulkanCommandBuffer& graphicsCommandBuffer = _graphicsCommandBuffers[_currentFrame];

        renderFinishedFence.wait();

        uint32_t imageIndex = UINT32_MAX;
        VkResult result = _swapchain.nextImage(imageAvailabeSemaphore, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            int width = 0;
            int height = 0;
            Application::window()->framebufferSize(&width, &height);
            recreateSwapchainAndFramebuffers(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            LOG_FAIL("Failed to acquire swapchain image!");
            throw std::runtime_error("");
        }

        renderFinishedFence.reset();

        const VulkanSemaphore& renderFinishedSemaphore = _renderFinishedSemaphore[imageIndex];

        graphicsCommandBuffer.reset();
        graphicsCommandBuffer.begin();

        // for (auto&& info : _bufferCopyInfos) {
        //     VkBufferMemoryBarrier barrier = {};
        //     barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        //     barrier.buffer = info.dstBuffer;
        //     barrier.offset = 0;
        //     barrier.size = VK_WHOLE_SIZE;
        //     barrier.srcQueueFamilyIndex = transferQueue().info().family;
        //     barrier.dstQueueFamilyIndex = graphicsQueue().info().family;
        //     barrier.srcAccessMask = 0;
        //     barrier.dstAccessMask = info.dstAccessMask;

        //     vkCmdPipelineBarrier(
        //         graphicsCommandBuffer,
        //         VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, info.dstStageMask,
        //         0,
        //         0, VK_NULL_HANDLE,
        //         1, &barrier,
        //         0, VK_NULL_HANDLE
        //     );
        // }

        _renderPass.begin(graphicsCommandBuffer, _framebuffers[imageIndex]);

        _trianglePipeline.bind(graphicsCommandBuffer);
        _trianglePipeline.bindVertexBuffer(graphicsCommandBuffer, { vertexBuffer });
        _trianglePipeline.bindIndexBuffer(graphicsCommandBuffer, indexBuffer);
        _trianglePipeline.drawIndexed(graphicsCommandBuffer, 3, 1, 0, 0, 0);
        // _trianglePipeline.draw(graphicsCommandBuffer, 3, 1, 0, 0);

        _renderPass.end(graphicsCommandBuffer);

        graphicsCommandBuffer.end();


        graphicsQueue().submit({ graphicsCommandBuffer }, { imageAvailabeSemaphore }, { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT }, { renderFinishedSemaphore }, renderFinishedFence);

        result = present(imageIndex, renderFinishedSemaphore);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _windowResized) {
            int width = 0;
            int height = 0;
            Application::window()->framebufferSize(&width, &height);
            recreateSwapchainAndFramebuffers(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
        }
        else if (result != VK_SUCCESS) {
            LOG_FAIL("Failed to present swap chain image!");
            throw std::runtime_error("");
        }

        _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void VulkanContext::onEvent(const Event& event) {
        switch (event.getEventType())
        {
        case Event::EventType::WindowResized:
            _windowResized = true;
            break;
        default:
            break;
        }
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

        for (auto&& layerName : _validationLayers)
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
                LOG_FAIL("\t" << "Required extension not found: " << requiredExtension);
            }
            else {
                LOG_INFO("\t" << "Required extension found: " << requiredExtension);
            }
        }

        if (success) {
            LOG_OK("\t" << "ALL required extensions found");
        }
        else {
            LOG_FAIL("\t" << "NOT ALL required extensions found");
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
        if (_enableValidationLayers && !checkValidationLayersSupport()) {
            LOG_FAIL("Validation layers were requested but are not supported!");
            throw std::runtime_error("");
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "NEWTONS";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.apiVersion = VK_VERSION;

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
        _surfaces.resize(1, VK_NULL_HANDLE);
        Application::window()->createVulkanSurface(_instance, nullptr, &_surfaces[0]);
    }

    void VulkanContext::pickPhysicalDevice() {

        if (_physicalDevices.size() == 0) {
            LOG_FAIL("Failed to find GPU with Vulkan support!");
            throw std::runtime_error("");
        }

        std::multimap<uint32_t, size_t> candidates;

        size_t index = 0;
        for (auto&& device : _physicalDevices) {
            uint32_t score = device.rating();
            candidates.insert(std::make_pair(score, index));
            index++;
        }

        if (candidates.rbegin()->first > 0) {
            _selectedPhysicalDeviceIndex = (candidates.rbegin()->second);
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(physicalDevice(), &props);
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
        if (_queues.size() > 0) {
            LOG_WARN("selecting queues even though existing!");
        }

        std::vector<VkQueueFamilyProperties> queueProps = physicalDevice().getAvailableQueueFamilyProperties();
        _queues.reserve(4);

        _queues.emplace_back(this, findGraphicsQueueInfo(queueProps));
        _queues.emplace_back(this, findPresentQueueInfo(queueProps));
        _queues.emplace_back(this, findTransferQueueInfo(queueProps));
    }

    void VulkanContext::createSwapchain() {
        int width = 0;
        int height = 0;
        Application::window()->framebufferSize(&width, &height);

        _swapchain = VulkanSwapchain(this);
        _swapchain.initialize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));

        LOG_INFO("Swapchain Successfully Created!\n");
    }

    void VulkanContext::initializeQueues() {
        for (auto&& queue : _queues) {
            queue.initialize();
        }
        LOG_SPACE();
    }

    void VulkanContext::createVmaAllocator() {
        VmaAllocatorCreateInfo info = {};
        info.instance = vkInstance();
        info.device = device();
        info.physicalDevice = physicalDevice();
        info.vulkanApiVersion = VK_VERSION;
        info.flags = 0;

        vmaCreateAllocator(&info, &_allocator);
    }

    void VulkanContext::createRenderPass() {
        _renderPass = VulkanRenderPass(this, { r, g, b });
        _renderPass.initialize();
    }

    void VulkanContext::createFramebuffers() {
        _framebuffers = FixedVector<VulkanFramebuffer>(_swapchain.vkImages().size());
        for (size_t i = 0; i < _framebuffers.size(); ++i) {
            _framebuffers[i] = VulkanFramebuffer(this);
            _framebuffers[i].initialize(_renderPass, _swapchain.vkImageViews()[i], _swapchain.vkExtent());
        }
        LOG_SPACE();
    }

    void VulkanContext::createSyncObjects() {
        _renderFinishedFences = FixedVector<VulkanFence>(MAX_FRAMES_IN_FLIGHT);
        _transferFinishedFences = FixedVector<VulkanFence>(MAX_FRAMES_IN_FLIGHT);
        _imageAvailableSemaphores = FixedVector<VulkanSemaphore>(MAX_FRAMES_IN_FLIGHT);
        _renderFinishedSemaphore = FixedVector<VulkanSemaphore>(_swapchain.vkImages().size());

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            _renderFinishedFences[i] = VulkanFence(this);
            _renderFinishedFences[i].initialize(true);

            _transferFinishedFences[i] = VulkanFence(this);
            _transferFinishedFences[i].initialize(false);

            _imageAvailableSemaphores[i] = VulkanSemaphore(this);
            _imageAvailableSemaphores[i].initialize();
        }

        LOG_SPACE();

        for (size_t i = 0; i < _renderFinishedSemaphore.size(); i++) {
            _renderFinishedSemaphore[i] = VulkanSemaphore(this);
            _renderFinishedSemaphore[i].initialize();
        }

        LOG_SPACE();
    }

    void VulkanContext::createCommandObjects() {
        _graphicsCommandPools = FixedVector<VulkanCommandPool>(MAX_FRAMES_IN_FLIGHT);
        for (auto&& commandPool : _graphicsCommandPools) {
            commandPool = VulkanCommandPool(this);
            commandPool.initialize(graphicsQueue().info().family, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
        }

        // LOG_INFO("Successfully Created Graphics Command Pools!");
        LOG_SPACE();

        _graphicsCommandBuffers = FixedVector<VulkanCommandBuffer>(MAX_FRAMES_IN_FLIGHT);
        size_t index = 0;
        for (auto&& commandBuffer : _graphicsCommandBuffers) {
            commandBuffer = VulkanCommandBuffer(this);
            commandBuffer.initialize(_graphicsCommandPools[index]);
            ++index;
        }

        // LOG_INFO("Successfully Created Graphics Command Buffers!");
        LOG_SPACE();

        _copyCommandPools = FixedVector<VulkanCommandPool>(MAX_FRAMES_IN_FLIGHT);
        for (auto&& pool : _copyCommandPools) {
            pool = VulkanCommandPool(this);
            pool.initialize(transferQueue().info().family, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
        }

        LOG_SPACE();

        _transferCommandBuffers = FixedVector<VulkanCommandBuffer>(MAX_FRAMES_IN_FLIGHT);
        index = 0;
        for (auto&& buffer : _transferCommandBuffers) {
            buffer = VulkanCommandBuffer(this);
            buffer.initialize(_copyCommandPools[index]);
            ++index;
        }

        LOG_SPACE();
    }

    VkResult VulkanContext::present(uint32_t imageIndex, const VulkanSemaphore& waitSemaphore) {
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        std::array<VkSemaphore, 1> waitSemaphores = { waitSemaphore };
        presentInfo.waitSemaphoreCount = waitSemaphores.size();
        presentInfo.pWaitSemaphores = waitSemaphores.data();

        std::array<VkSwapchainKHR, 1> swapChains = { _swapchain };
        presentInfo.swapchainCount = swapChains.size();
        presentInfo.pSwapchains = swapChains.data();

        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        return vkQueuePresentKHR(presentQueue(), &presentInfo);
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

    void VulkanContext::copyBuffer(const VulkanBuffer& srcBuffer, const VulkanBuffer& dstBuffer, VkPipelineStageFlags pipelineStage, VkAccessFlags accessFlag) {
        _bufferCopyInfos.emplace_back(srcBuffer, dstBuffer, pipelineStage, accessFlag);
    }

    // std::shared_ptr<VulkanBuffer> VulkanContext::createBuffer() {
    //     return std::make_shared<VulkanBuffer>(this);
    // }

    VulkanQueueInfo VulkanContext::findPresentQueueInfo(const std::vector<VkQueueFamilyProperties>& availableQueueFamilyProps) const {
        VulkanQueueInfo queueInfo(-1, 0);

        // uint32_t savedFamily = -1;
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

            // // puts all queue indices of "usedQueues" that have the save queue family as the current queue family in an "unordered_set"
            // std::unordered_set<uint32_t> usedQueueIndices;
            // for (auto&& usedQueue : _queues) {
            //     if (usedQueue.info().family == familyIndex) {
            //         usedQueueIndices.emplace(usedQueue.info().index);
            //     }
            // }

            // // checks for availavle queue indices in the set
            // // if an index is free, select current queue family and free queue index
            // for (uint32_t i = 0; i < familyProp.queueCount; i++) {
            //     if (!usedQueueIndices.contains(i)) {
            //         queueInfo.family = familyIndex;
            //         queueInfo.index = i;
            //         return queueInfo;
            //     }
            // }

            // select current queue family as fallback if none was selected
            // if (savedFamily == -1) {
            //     savedFamily = familyIndex;
            // }

            familyIndex++;
        }

        // fallback if no unique queue family was found
        if (queueInfo.family == -1) {
            queueInfo.family = graphicsQueue().info().family;
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

        // return graphicsQueue().info();

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

    void VulkanContext::recreateSwapchainAndFramebuffers(uint32_t width, uint32_t height) {
        for (auto&& fence : _renderFinishedFences) {
            fence.wait();
        }

        _swapchain.destroy();
        _swapchain = VulkanSwapchain(this);
        _swapchain.initialize(width, height);

        for (size_t i = 0; i < _framebuffers.size(); i++) {
            _framebuffers[i].destroy();
            _framebuffers[i] = VulkanFramebuffer(this);
            _framebuffers[i].initialize(_renderPass, _swapchain.vkImageViews()[i], _swapchain.vkExtent());
        }

        _windowResized = false;
    }
} // namespace nwt