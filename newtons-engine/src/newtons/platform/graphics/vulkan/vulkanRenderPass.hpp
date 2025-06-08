#pragma once

#include "newtons/pch.hpp"
#include <vulkan/vulkan.h>


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

        // std::vector<VkFramebuffer> _vkFramebuffers;

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
        // const std::vector<VkFramebuffer>& vkFramebuffers() const;

        void setClearColor(VkClearColorValue clearColor);

        void initialize();
        void destroy();

        void begin(const VulkanFrameInfo& frameInfo, uint32_t imageIndex);
        void end(const VulkanFrameInfo& frameInfo);
    };
} // namespace nwt
