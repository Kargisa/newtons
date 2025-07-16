#pragma once

#include "vulkan/vulkan.h"

#include "newtons/pch.hpp"
#include "vec3.hpp"

namespace nwt
{
    struct VulkanVertexInfo {
        VulkanVertexInfo() = delete;

        static constexpr std::array<VkVertexInputBindingDescription, 1> bindingDescriptions() {
            constexpr uint32_t binding_0 = 0;
            constexpr uint32_t binding_1 = 1;
            constexpr uint32_t binding_2 = 2;

            return
            {
                VkVertexInputBindingDescription{ binding_0, sizeof(Vec3), VK_VERTEX_INPUT_RATE_VERTEX }
                // VkVertexInputBindingDescription{ binding_1, sizeof(Vec3), VK_VERTEX_INPUT_RATE_VERTEX },
                // VkVertexInputBindingDescription{ binding_2, sizeof(Vec2), VK_VERTEX_INPUT_RATE_VERTEX }
            };
        }

        static constexpr std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions() {
            constexpr uint32_t location_0 = 0;
            constexpr uint32_t location_1 = 1;
            constexpr uint32_t location_2 = 2;

            constexpr uint32_t binding_0 = 0;
            constexpr uint32_t binding_1 = 1;
            constexpr uint32_t binding_2 = 2;

            constexpr uint32_t offset = 0;

            return
            {
                VkVertexInputAttributeDescription{ location_0, binding_0, VK_FORMAT_R32G32B32_SFLOAT, offset }
                // VkVertexInputAttributeDescription{ location_1, binding_1, VK_FORMAT_R32G32B32_SFLOAT, offset },
                // VkVertexInputAttributeDescription{ location_2, binding_2, VK_FORMAT_R32G32_SFLOAT, offset }
            };
        }
    };
} // namespace nwt
