#include "vulkanRenderPass.hpp"
#include "vulkanContext.hpp"
namespace nwt
{
    VkRenderPass VulkanRenderPass::vkRenderPass() const {
        return _renderPass;
    }

    const VkClearColorValue& VulkanRenderPass::vkClearColor() const {
        return _clearColor;
    }

    const VkClearDepthStencilValue& VulkanRenderPass::vkClearDepthStencil() const {
        return _clearDepthStencil;
    }

    const VkRect2D& VulkanRenderPass::vkRenderArea() const {
        return _renderArea;
    }

    // const std::vector<VkFramebuffer>& VulkanRenderPass::vkFramebuffers() const {
    //     return _vkFramebuffers;
    // }

    void VulkanRenderPass::setClearColor(VkClearColorValue clearColor) {
        _clearColor = clearColor;
    }

    void VulkanRenderPass::initialize() {
        _clearColor = { 0.392f, 0.584f, 0.929f };

        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = _context->swapchain().vkImageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        // VkAttachmentDescription depthAttachment{};
        // depthAttachment.format = depthFormat;
        // depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        // depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        // depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        // depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        // depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        // depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        // depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        // VkAttachmentReference depthAttachmentRef{};
        // depthAttachmentRef.attachment = 1;
        // depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        // subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        // std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
        std::array<VkAttachmentDescription, 1> attachments = { colorAttachment };

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(_context->device(), &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS) {
            throw std::runtime_error("Unable To Create Renderpass!");
        }

        LOG_INFO("Render Pass Successfully Created!");
    }

    void VulkanRenderPass::begin(const VulkanFrameInfo& frameInfo, uint32_t imageIndex) {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = _renderPass;
        renderPassInfo.framebuffer = _context->framebuffers()[imageIndex];

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = _context->swapchain().vkExtent();

        std::array<VkClearValue, 1> clearValues{};
        clearValues[0].color = _clearColor;
        // clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(frameInfo.commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VulkanRenderPass::end(const VulkanFrameInfo& frameInfo) {
        vkCmdEndRenderPass(frameInfo.commandBuffer);
    }

    void VulkanRenderPass::destroy() {
        if (_renderPass == VK_NULL_HANDLE) {
            return;
        }

        VkDevice device = _context->device().vkDevice();

        vkDestroyRenderPass(device, _renderPass, nullptr);
        // for (auto&& framebuffer : _vkFramebuffers) {
        //     vkDestroyFramebuffer(device, framebuffer, nullptr);
        // }

        _renderPass = VK_NULL_HANDLE;
        _context = nullptr;
        // _vkFramebuffers.clear();

        LOG_INFO("Render Pass Destroyed!");

    }
} // namespace nwt

