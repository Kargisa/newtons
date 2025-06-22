#pragma once

#include <vulkan/vulkan.h>

#include "newtons/pch.hpp"
#include "fixedVector.hpp"
#include "vulkanFramebuffer.hpp"
#include "vulkanCommandBuffer.hpp"


namespace nwt
{
    struct VulkanContext;
    struct VulkanFrameInfo;

    class VulkanRenderPass {

        VulkanContext* _context;
        VkRenderPass _renderPass;
        VkClearColorValue _clearColor;
        VkClearDepthStencilValue _clearDepthStencil;
        VkRect2D _renderArea;

    public:
        VulkanRenderPass()
            : _context(nullptr), _renderPass(VK_NULL_HANDLE) {
        }
        VulkanRenderPass(VulkanContext* context, VkClearColorValue clearColor)
            : _context(context), _clearColor(clearColor), _renderPass(VK_NULL_HANDLE) {
        }


        VkRenderPass vkRenderPass() const;
        const VkClearColorValue& vkClearColor() const;
        const VkClearDepthStencilValue& vkClearDepthStencil() const;
        const VkRect2D& vkRenderArea() const;

        void setClearColor(VkClearColorValue clearColor);

        void initialize();
        void destroy();

        void begin(const VulkanCommandBuffer& commandBuffer, const VulkanFramebuffer& framebuffer);
        void end(const VulkanCommandBuffer& commandBuffer);
    };
} // namespace nwt
