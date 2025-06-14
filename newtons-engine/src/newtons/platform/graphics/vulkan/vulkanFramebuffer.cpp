#include "vulkanFramebuffer.hpp"
#include "vulkanContext.hpp"

namespace nwt
{

    void VulkanFramebuffer::initialize(const VulkanRenderPass& renderPass, VkImageView colorImageView, VkExtent2D extent) {
        std::array<VkImageView, 1> attachments = {
            colorImageView,
            // _depthImageView
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass.vkRenderPass();
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(_context->device(), &framebufferInfo, nullptr, &_framebuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }

        LOG_INFO("Framebuffer Successfully Created!");
    }

    void VulkanFramebuffer::destroy() {
        if (_framebuffer == VK_NULL_HANDLE) {
            return;
        }

        vkDestroyFramebuffer(_context->device().vkDevice(), _framebuffer, nullptr);
        _framebuffer = VK_NULL_HANDLE;

        LOG_INFO("Framebuffer Destroyed!");
    }
    VkFramebuffer VulkanFramebuffer::vkFramebuffer() const {
        return _framebuffer;
    }

    VulkanFramebuffer::operator VkFramebuffer() const {
        return _framebuffer;
    }
} // namespace nwt
