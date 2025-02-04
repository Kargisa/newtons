
#include "newtons/pch.hpp"
#include "vulkanRenderer.hpp"
#include "newtons/application.hpp"
#include <cstring>

nwt::VulkanRenderer::VulkanRenderer(const char** thirdPartyExtrensions, size_t thirdPartySize)
{
    createInstance(thirdPartyExtrensions, thirdPartySize);
}

nwt::VulkanRenderer::~VulkanRenderer()
{
}

bool nwt::VulkanRenderer::checkValidationLayersSupport()
{
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : _validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (std::strcmp(layerName, layerProperties.layerName)) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
            return false;
    }

    return true;
}

bool nwt::VulkanRenderer::checkExtensionsSupport(const std::vector<const char*>& requiredExtensions, const std::vector<VkExtensionProperties>& extensions)
{
    LOG_INFO("Extensions:");

    for (const auto& extension : extensions)
        LOG_INFO("\t" << extension.extensionName);

    LOG_INFO("\n")

    LOG_INFO("Required extensions:");

    uint16_t foundExtensionsCount = 0;

    bool success = true;

    for (const auto& requiredExtension : requiredExtensions)
    {
        bool found = false;
        for (const auto& extension : extensions)
        {
            if (std::strcmp(requiredExtension, extension.extensionName)) {
                found = true;
                foundExtensionsCount++;
                break;
            }
        }

        if (!found)
            success = false;

        LOG_INFO("\t" << "Required extension" << RED_COLOR << (found ? " " : " NOT ") << WHITE_COLOR << "found: " << requiredExtension);
    }

    if (foundExtensionsCount == requiredExtensions.size()) {
        LOG_INFO("\t" << GREEN_COLOR << "ALL " << WHITE_COLOR << "required extensions found");
    }
    else {
        LOG_INFO("\t" << RED_COLOR << "NOT ALL " << WHITE_COLOR << "required extensions found");
    }

    return success;
}

void nwt::VulkanRenderer::createInstance(const char** thirdPartyExtrensions, size_t thirdPartySize)
{
    if (_enableValidationLayers && !checkValidationLayersSupport())
        throw std::runtime_error("Validation layers requested, but not available!");

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "NEWTONS";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if constexpr (_enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
        createInfo.ppEnabledLayerNames = _validationLayers.data();
    }
    else
        createInfo.enabledLayerCount = 0;

    std::vector<const char*> requiredExtensions(thirdPartySize);

    for (uint32_t i = 0; i < thirdPartySize; i++){
        requiredExtensions[i] = thirdPartyExtrensions[i];
    }

    requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    createInfo.enabledExtensionCount = requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();
    //createInfo.enabledLayerCount = 0;

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    if (!checkExtensionsSupport(requiredExtensions, extensions))
        throw std::runtime_error("Required extensions not found!");

    VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);

    if (result != VK_SUCCESS)
        throw std::runtime_error("failed to create instance!");

}

void nwt::VulkanRenderer::createSurface()
{
    Application::instace()->getWindow()->createVulkanSurface(_instance, nullptr, &_surface);
}

nwt::VulkanRenderer* nwt::VulkanRenderer::create(const char** thirdPartyExtrensions, size_t thirdPartySize)
{
    return new VulkanRenderer(thirdPartyExtrensions, thirdPartySize);
}
