# if (WIN32)
# add_executable (newtons-editor WIN32 "main.cpp" "obj_reader.hpp" "vertex.hpp" "vertex.cpp" "transformationMatrices.hpp" "mesh.cpp" "mesh.hpp" "stb_image.h")
# else()
add_library(newtons-engine STATIC 
"newtons/main.cpp" 
"newtons/mesh.cpp" 
"newtons/application.cpp" 
"newtons/window/window.cpp" 
"newtons/window/window.hpp" 
"newtons/graphics/graphicsContext.cpp"
"newtons/graphics/graphicsFrame.cpp" 
"newtons/graphics/graphics.cpp" 
"newtons/platform/graphics/vulkan/vulkanContext.cpp" 
"newtons/platform/linux/linuxWindow.cpp" 
"newtons/platform/windows/windowsWindow.cpp" 
"newtons/platform/graphics/vulkan/vulkanVertex.cpp" 
"newtons/platform/graphics/vulkan/vulkanShader.cpp"
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
)
# endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET newtons-engine PROPERTY CXX_STANDARD 26)
endif()

find_package(Vulkan REQUIRED)

target_link_libraries(newtons-engine PRIVATE newtons-utils PUBLIC glfw PUBLIC Vulkan::Vulkan)

target_include_directories(newtons-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(newtons-engine PRIVATE "newtons/pch.hpp")

target_compile_definitions(newtons-engine PRIVATE NWT_EXPORTS)
