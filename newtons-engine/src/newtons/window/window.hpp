#pragma once

#include "newtons/pch.hpp"
#include "newtons/event/mouseEvent.hpp"
#include "newtons/event/keyEvent.hpp"
#include "newtons/event/windowEvent.hpp"

#include <vulkan/vulkan.h>

#include <functional>

namespace nwt {

    typedef std::function<void(const Event&)> EventFunc;

    struct WindowData {
        const char* name;
        EventFunc eventCallback;
    };

    class Window {
    protected:
        WindowData _data;

        Window() = default;
    public:
        virtual ~Window() = default;

        virtual void initialize(int width, int height) = 0;
        virtual void createCallbacks() = 0;

        virtual void setEventCallback(EventFunc func) = 0;
        virtual void pollEvenets() = 0;

        virtual void* nativeWindow() = 0;
        virtual void framebufferSize(int* width, int* height) = 0;

        virtual void destroy() = 0;

        // ********************************
        // ************ Vulkan ************
        // ********************************

        virtual void createVulkanSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) = 0;
        virtual const char** getVulkanExtensions(uint32_t* count) = 0;
    };


} // namespace nwt