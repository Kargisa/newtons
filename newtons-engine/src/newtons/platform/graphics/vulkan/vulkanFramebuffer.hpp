#pragma once

#include "vulkan/vulkan.h"
#include "newtons/pch.hpp"
#include "fixedVector.hpp"

namespace nwt
{
    struct VulkanContext;
    struct VulkanRenderPass;

    class VulkanFramebuffer {
        VulkanContext* _context;
        VkFramebuffer _framebuffer;

    public:
        VulkanFramebuffer()
            : _context(nullptr), _framebuffer(VK_NULL_HANDLE) {
        }

        VulkanFramebuffer(VulkanContext* context)
            : _context(context), _framebuffer(VK_NULL_HANDLE) {
        }

        void initialize(const VulkanRenderPass& renderPass, VkImageView colorImageView, VkExtent2D extent);
        void destroy();

        VkFramebuffer vkFramebuffer() const;
        operator VkFramebuffer() const;

    };
} // namespace nwt
