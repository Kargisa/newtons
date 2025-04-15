#include "graphicsContext.hpp"
#include "newtons/platform/renderer/vulkan/vulkanContext.hpp"


namespace nwt
{
    GraphicsAPI GraphicsContext::getAPI() const
    {
        return _api;
    }

    GraphicsContext* GraphicsContext::create(GraphicsAPI api)
    {
        switch (api)
        {
        case GraphicsAPI::VULKAN_API:
            auto context = new VulkanContext();
            context->_api = api;
            return context;
        default:
            break;
        }
    }


} // namespace nwt
