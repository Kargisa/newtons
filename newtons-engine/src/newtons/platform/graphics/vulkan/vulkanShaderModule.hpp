#pragma once

#include "newtons/pch.hpp"
#include "vulkan/vulkan.h"

namespace nwt
{
    struct VulkanContext;

    class VulkanShaderModule {
        VulkanContext* _context;
        VkShaderModule _shaderModule;

    public:
        VulkanShaderModule()
            : _context(nullptr), _shaderModule(VK_NULL_HANDLE) {
        }

        VulkanShaderModule(VulkanContext* context)
            : _context(context), _shaderModule(VK_NULL_HANDLE) {
        }

        VkShaderModule vkShaderModule() const;
        operator VkShaderModule() const;

        void initialize(uint32_t* code, size_t codeSize);
        void destroy();
    };
} // namespace nwt
