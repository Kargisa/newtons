
#include "imguiLayer.hpp"
#include "newtons/application.hpp"

#include "external/imgui.h"
#include "external/imgui_impl_glfw.h"
#include "external/imgui_impl_vulkan.h"
#include "newtons/platform/graphics/vulkan/vulkanContext.hpp"

namespace nwt
{
    void ImguiLayer::onAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(reinterpret_cast<GLFWwindow*>(Application::window()->nativeWindow()), true);

        VulkanContext* vulkanContext = reinterpret_cast<VulkanContext*>(Application::graphcisContext()->nativeContext());

        ImGui_ImplVulkan_InitInfo info = {};
        info.Instance = vulkanContext->vkInstance();
        info.PhysicalDevice = vulkanContext->physicalDevice();
        info.Device = vulkanContext->device();
        info.QueueFamily = vulkanContext->graphicsQueue().info().family;
        info.Queue = vulkanContext->graphicsQueue();
        // info.PipelineCache = YOUR_PIPELINE_CACHE;
        // info.DescriptorPool = YOUR_DESCRIPTOR_POOL;
        info.Subpass = 0;
        info.MinImageCount = 2;
        info.ImageCount = 2;
        info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        // info.Allocator = YOUR_ALLOCATOR;
        // info.CheckVkResultFn = check_vk_result;

        ImGui_ImplVulkan_Init(&info);
    }

    void ImguiLayer::onDetach() {

    }

    void ImguiLayer::onEvent(const Event& event) {

    }

    void ImguiLayer::update() {

    }


} // namespace nwt