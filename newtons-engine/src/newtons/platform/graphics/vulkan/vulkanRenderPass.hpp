#pragma once

#include "newtons/pch.hpp"
#include <vulkan/vulkan.h>

namespace nwt
{
    struct VulkanContext;

    class VulkanRenderPass {

        VulkanContext* _context;
        VkRenderPass _vkRenderPass;
        VkClearColorValue _vkClearColor;
        VkClearDepthStencilValue _vkClearDepthStencil;
        VkRect2D _vkRenderArea;

        std::vector<VkFramebuffer> _vkFramebuffers;

    public:
        VulkanRenderPass()
            : _context(nullptr), _vkRenderPass(nullptr) {
        }
        VulkanRenderPass(VulkanContext* context)
            : _context(context), _vkRenderPass(nullptr) {
        }


        VkRenderPass vkRenderPass() const;
        const VkClearColorValue& vkClearColor() const;
        const VkClearDepthStencilValue& vkClearDepthStencil() const;
        const VkRect2D& vkRenderArea() const;
        const std::vector<VkFramebuffer>& vkFramebuffers() const;



        void initialize();
        void destroy();

        void begin(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void end(VkCommandBuffer commandBuffer);
    };
} // namespace nwt
