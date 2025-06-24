#include "vulkanShaderModule.hpp"
#include "vulkanContext.hpp"

namespace nwt
{
    void VulkanShaderModule::initialize(uint32_t* code, size_t codeSize) {
        VkShaderModuleCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        info.codeSize = codeSize;
        info.pCode = code;

        if (vkCreateShaderModule(_context->device(), &info, nullptr, &_shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("Could not create shader module!");
        }
    }

    VkShaderModule VulkanShaderModule::vkShaderModule() const {
        return _shaderModule;
    }

    VulkanShaderModule::operator VkShaderModule() const {
        return _shaderModule;
    }

    void VulkanShaderModule::destroy() {
        if (_shaderModule == VK_NULL_HANDLE) {
            return;
        }

        vkDestroyShaderModule(_context->device(), _shaderModule, nullptr);
    }
} // namespace nwt
