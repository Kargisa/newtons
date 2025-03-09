#pragma once

#include "newtons/pch.hpp"
#include "newtons/window/window.hpp"
#include <GLFW/glfw3.h>

namespace nwt{
    class LinuxWindow : public Window{
        GLFWwindow* _window;

        LinuxWindow(int width, int height, const char* name);

    public:
        virtual ~LinuxWindow();

        virtual void init() override;
        virtual void createCallbacks() override;
        virtual void* getNativeWindow() override;
        virtual void getFramebufferSize(int* width, int* height) override;

        virtual void destroy() override;
    
        virtual void createVulkanSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) override;
        virtual const char** getVulkanExtensions(uint32_t* count) override;

        static LinuxWindow* create(int width, int height, const char* name);

    };
} // namespace nwt