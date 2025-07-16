
add_library(newtons-engine STATIC 
"newtons/application.cpp"
"newtons/layerStack.cpp"
"newtons/main.cpp"
"newtons/mesh.cpp"
"newtons/io/file.cpp"
"newtons/window/window.cpp"
"newtons/window/window.hpp"
"newtons/graphics/graphicsContext.cpp"
"newtons/graphics/graphicsFrame.cpp"
"newtons/graphics/graphics.cpp"
"newtons/platform/graphics/vulkan/vulkanContext.cpp"
"newtons/platform/linux/linuxWindow.cpp"
"newtons/platform/windows/windowsWindow.cpp"
"newtons/platform/graphics/vulkan/vulkanVertex.cpp" 
"newtons/platform/graphics/vulkan/vulkanSwapchain.cpp"
"newtons/platform/graphics/vulkan/vulkanRenderPass.cpp"
"newtons/platform/graphics/vulkan/vulkanPhysicalDevice.cpp"
"newtons/platform/graphics/vulkan/vulkanDevice.cpp"
"newtons/platform/graphics/vulkan/vulkanCommandPool.cpp"
"newtons/platform/graphics/vulkan/vulkanCommandBuffer.cpp"
"newtons/platform/graphics/vulkan/vulkanFramebuffer.cpp"
"newtons/platform/graphics/vulkan/vulkanSemaphore.cpp"
"newtons/platform/graphics/vulkan/vulkanFence.cpp"
"newtons/platform/graphics/vulkan/vulkanQueue.cpp"
"newtons/platform/graphics/vulkan/vulkanGraphicsPipeline.cpp"
"newtons/platform/graphics/vulkan/vulkanShaderModule.cpp"
"newtons/platform/graphics/vulkan/vulkanReflect.cpp"
"newtons/platform/graphics/vulkan/vulkanBuffer.cpp"
"newtons/platform/graphics/vulkan/vulkanVertexInfo.cpp"

#VMA
"newtons/platform/graphics/vulkan/vmaUsages.cpp"



# Dear IMGUI
"newtons/platform/imgui/external/imgui.cpp"
"newtons/platform/imgui/external/imgui_demo.cpp"
"newtons/platform/imgui/external/imgui_draw.cpp"
"newtons/platform/imgui/external/imgui_impl_glfw.cpp"
"newtons/platform/imgui/external/imgui_impl_vulkan.cpp"
"newtons/platform/imgui/external/imgui_tables.cpp"
"newtons/platform/imgui/external/imgui_widgets.cpp"
"newtons/platform/imgui/imguiLayer.cpp"

)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET newtons-engine PROPERTY CXX_STANDARD 23)
endif()

find_package(Vulkan REQUIRED)

target_link_libraries(newtons-engine 
PUBLIC newtons-utils 
PUBLIC glfw 
PUBLIC Vulkan::Vulkan 
PUBLIC GPUOpen::VulkanMemoryAllocator
PUBLIC spirv-reflect-static
)


target_include_directories(newtons-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(newtons-engine PRIVATE "newtons/pch.hpp")

target_compile_definitions(newtons-engine PRIVATE NWT_EXPORTS)
