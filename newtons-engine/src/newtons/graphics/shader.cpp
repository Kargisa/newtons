#include "shader.hpp"
#include "newtons/application.hpp"
#include "newtons/platform/graphics/vulkan/vulkanShader.hpp"
#include "file.hpp"

namespace nwt {
    Shader* Shader::create(const std::filesystem::path& path) {


        switch (Application::instance()->getGraphcisContext()->getAPI()) {
        case GraphicsAPI::VULKAN_API:
            return new VulkanShader();
            break;
        }
    }

    Shader* Shader::find(const std::string& name) {
        Shader* shader;
        auto itr = _shaders.find(name);
        if (itr == _shaders.end()) {
            return nullptr;
        }

        return &itr->second();
    }

    bool Shader::setRootFolder(const std::filesystem::path& path) {
        if (!File::isDirectory(path)) {
            return false;
        }

        _root = path;
    }


} // namespace nwt

