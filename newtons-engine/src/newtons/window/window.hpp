#pragma once

#include "newtons/pch.hpp"
#include "newtons/event/mouseEvent.hpp"
#include "newtons/event/keyEvent.hpp"
#include "newtons/event/windowEvent.hpp"

#include <vulkan/vulkan.h>

#include <functional>

namespace nwt{

    typedef std::function<void(const Event&)> EventFunc;

    struct WindowData{
        const char* name;
        int width;
        int height;
        EventFunc eventCallback;
    };

    class Window {
    protected:
        WindowData _data;

        Window() = default;
    public:
        virtual ~Window() = default;

    private:
        virtual void init() = 0;
        virtual void createCallbacks() = 0;

    public:
        virtual void setEventCallback(EventFunc func);
        virtual int getWidth() const;
        virtual int getHeight() const;

        virtual void* getNativeWindow() = 0;
        virtual void getFramebufferSize(int* width, int* height) = 0;

        virtual void destroy() = 0;

        // ********************************
        // ************ Vulkan ************
        // ********************************

        virtual void createVulkanSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) = 0;
        virtual const char** getVulkanExtensions(uint32_t* count) = 0;
    };


} // namespace nwt