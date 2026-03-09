#include "graphicsContext.hpp"
#include "newtons/platform/graphics/vulkan/vulkanContext.hpp"


namespace nwt
{

    GraphicsContext* GraphicsContext::create(GraphicsAPI api) {
        switch (api)
        {
        case GraphicsAPI::VULKAN:
            VulkanContext* context = new VulkanContext();
            return context;
        }
        throw std::runtime_error("no api found!");
    }


} // namespace nwt
