#pragma once

#include "vulkan/vulkan.h"
#include "newtons/pch.hpp"

#include "vulkanShaderModule.hpp"
#include "vulkanCommandBuffer.hpp"

namespace nwt
{
    struct VulkanContext;
    class VulkanGraphicsPipeline {
        VulkanContext* _context;
        VkPipelineLayout _pipelineLayout;
        VkPipeline _pipeline;

    public:
        VulkanGraphicsPipeline()
            : _context(nullptr) {
        }

        VulkanGraphicsPipeline(VulkanContext* context)
            : _context(context) {
        }

        void initialize(uint32_t* vertCode, size_t vertCodeSize, uint32_t* fragCode, size_t fragCodeSize);
        void destroy();

        void bind(const VulkanCommandBuffer& commandBuffer) const;
        void draw(const VulkanCommandBuffer& commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const;
    };
} // namespace nwt
