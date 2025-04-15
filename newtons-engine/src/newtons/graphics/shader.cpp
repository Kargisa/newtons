#include "shader.hpp"
#include "newtons/application.hpp"
#include "newtons/platform/graphics/vulkan/vulkanShader.hpp"
#include "file.hpp"

namespace nwt {
    Shader* Shader::create(const std::filesystem::path& vertPath, const std::filesystem::path& fragPath, const std::string& name) {
        // ************* Data is not persistent *************

        // switch (Application::instance()->getGraphcisContext()->getAPI()) {
        // case GraphicsAPI::VULKAN_API:
        //     auto pair = _shaders.try_emplace(name, VulkanShader(vertPath, fragPath));
        //     if (pair.second) {
        //         return &pair.first->second;
        //     }

        //     return nullptr;
        // }

        // return nullptr;
    }

    Shader* Shader::find(const std::string& name) {
        // ************* Data is not persistent *************

        // auto itr = _shaders.find(name);
        // if (itr == _shaders.end()) {
        //     return nullptr;
        // }

        // return &itr->second;
    }

    uint32_t* Shader::loadShader(ShaderType type, uint32_t* const size) const
    {
        uint32_t* buffer = nullptr;
        size_t fileSize = 0;

        switch (type)
        {
        case VulkanShader::ShaderType::VERTEX:
            buffer = reinterpret_cast<uint32_t*>(File::readBinary((_vertPath), fileSize));
            break;
        case VulkanShader::ShaderType::FRAGMENT:
            buffer = reinterpret_cast<uint32_t*>(File::readBinary((_fragPath), fileSize));
            break;
        }

        *size = static_cast<uint32_t>(fileSize);
        return buffer;
    }
} // namespace nwt

