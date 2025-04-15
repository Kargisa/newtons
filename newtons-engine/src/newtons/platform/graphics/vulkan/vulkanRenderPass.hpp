#pragma once

#include <vulkan/vulkan.h>

namespace nwt
{
    class VulkanRenderPass {
        VkDevice _device;
        VkRenderPass _vkRenderPass;
        VkClearColorValue _clearColor;
        VkClearDepthStencilValue _clearDepthStencil;
        VkRect2D _renderArea;
        std::vector<VkFramebuffer> _framebuffers;

    public:
        VulkanRenderPass() = default;
        VulkanRenderPass(VkDevice device, VkClearColorValue clearColor, VkClearDepthStencilValue clearDepthStencil, VkRect2D renderArea)
            : _device(device), _clearColor(clearColor), _clearDepthStencil(clearDepthStencil), _renderArea(renderArea) {
        }

        virtual ~VulkanRenderPass();

        VkRenderPass getVkRenderPass() const;
        const VkClearColorValue& getClearColor() const;
        const VkClearDepthStencilValue& getClearDepthStencil() const;
        const VkRect2D& getRenderArea() const;
        const std::vector<VkFramebuffer>& getFramebuffers() const;



        VkResult create(VkFormat presentFormat, VkFormat depthFormat);
        void begin(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer);
        void end(VkCommandBuffer commandBuffer);

        void destroy();
    };
} // namespace nwt
