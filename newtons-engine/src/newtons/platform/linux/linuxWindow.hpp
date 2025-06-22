#pragma once

#include "newtons/pch.hpp"
#include "newtons/window/window.hpp"
#include <GLFW/glfw3.h>

namespace nwt {
    class LinuxWindow : public Window {
        GLFWwindow* _window;

        LinuxWindow(const char* name);

    public:
        virtual ~LinuxWindow();

        virtual void initialize(int width, int height) override;
        virtual void createCallbacks() override;
        virtual void* nativeWindow() override;
        virtual void framebufferSize(int* width, int* height) override;
        virtual void setEventCallback(EventFunc func) override;
        virtual void pollEvenets() override;

        virtual void destroy() override;

        virtual void createVulkanSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) override;
        virtual const char** getVulkanExtensions(uint32_t* count) override;

        static LinuxWindow* create(const char* name);

    };
} // namespace nwt