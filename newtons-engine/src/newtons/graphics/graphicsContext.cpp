#include "graphicsContext.hpp"
#include "newtons/platform/graphics/vulkan/vulkanContext.hpp"


namespace nwt
{
    GraphicsAPI GraphicsContext::getAPI() const {
        return _api;
    }

    GraphicsContext* GraphicsContext::create(GraphicsAPI api) {
        switch (api)
        {
        case GraphicsAPI::VULKAN:
            VulkanContext* context = new VulkanContext();
            context->_api = api;
            return context;
        }
        throw std::runtime_error("no api found!");
    }


} // namespace nwt
