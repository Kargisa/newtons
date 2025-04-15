#pragma once

namespace nwt
{
    struct VulkanDepthBuffer {
        VkImage image;
        VkImageView imageView;
        VkDeviceMemory memory;

        VulkanDepthBuffer() = default;
        VulkanDepthBuffer(const VkImage& image, const VkImageView& imageView, const VkDeviceMemory& memory)
            : image(image), imageView(imageView), memory(memory) {
        }
    };
} // namespace nwt