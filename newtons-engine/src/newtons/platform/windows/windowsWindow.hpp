#pragma once

#include "newtons/pch.hpp"
#include "newtons/window/window.hpp"
#include <GLFW/glfw3.h>

namespace nwt {
    class WindowsWindow : public Window {
        GLFWwindow* _window;

        WindowsWindow(int width, int height, const char* name);

    public:
        virtual ~WindowsWindow();

        virtual void init() override;
        virtual void createCallbacks() override;
        virtual void* getNativeWindow() override;
        virtual void framebufferSize(int* width, int* height) override;

        virtual void destroy() override;

        virtual void createVulkanSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) override;
        virtual const char** getVulkanExtensions(uint32_t* count) override;

        static WindowsWindow* create(int width, int height, const char* name);

    };
} // namespace nwt