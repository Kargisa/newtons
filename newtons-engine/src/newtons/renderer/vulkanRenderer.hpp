#pragma once

#include "../pch.hpp"
#include <vulkan/vulkan.h>
namespace nwt{
class NWT_API VulkanRenderer{

#ifdef DEBUG
	const std::vector<const char*> _validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};
	static constexpr bool _enableValidationLayers = true;
#else
    const std::vector<const char*> _validationLayers(0);
	static constexpr bool _enableValidationLayers = false;
#endif

    VkInstance _instance;
    VkSurfaceKHR _surface;

    VulkanRenderer(const char** thirdPartyExtrension, size_t thirdPartySize);
public:
    ~VulkanRenderer();

private:
    bool checkValidationLayersSupport();
    bool checkExtensionsSupport(const std::vector<const char*>& requiredExtensions, const std::vector<VkExtensionProperties>& extensions);

    void createInstance(const char** thirdPartyExtrensions, size_t thirdPartySize);
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();

public:
    static VulkanRenderer* create(const char** thirdPartyExtrensions, size_t thirdPartySize);

};
} // namespace nwt