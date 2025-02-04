#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "windowsWindow.hpp"

namespace nwt
{
    WindowsWindow::WindowsWindow(int width, int height, const char* name) {
        _data.width = width;
        _data.height = height;
        _data.name = name;

        init();
        createCallbacks();
    }

    WindowsWindow::~WindowsWindow()
    {
        glfwDestroyWindow(_window);
    }

    void WindowsWindow::init()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        _window = glfwCreateWindow(_data.width, _data.height, _data.name, nullptr, nullptr);
        glfwSetWindowUserPointer(_window, &_data);
    }

    void WindowsWindow::createCallbacks()
    {
        glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height){
            WindowData data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.height = height;
            data.width = width;

            WindowResizedEvent event(data.width, data.height);
            data.eventCallback(event);
        });

        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window){
            WindowData data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.eventCallback(WindowClosedEvent());
        });

        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            WindowData data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            
            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyPressedEvent event(key);
                data.eventCallback(event);    
                break;
            }
            case GLFW_REPEAT:
            {                
                KeyPressedEvent event(key);
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(key);
                data.eventCallback(event);
                break;
            }
            }
        });

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double x, double y){
            WindowData data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            
            MouseMovedEvent event(static_cast<float>(x), static_cast<float>(y));
            data.eventCallback(event);
        });

        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods){
            WindowData data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
            {
                MousePressedEvent event(button);
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseReleasedEvent event(button);
                data.eventCallback(event);
                break;
            }
            }
        });

        glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset){
            WindowData data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

            MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            data.eventCallback(event);
        });
    }

    void WindowsWindow::createVulkanSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) {
        glfwCreateWindowSurface(instance, _window, allocator, surface);
    }

    void *WindowsWindow::getNativeWindow()
    {
        return _window;
    }

    WindowsWindow* WindowsWindow::create(int width, int height, const char* name = "NEWTONS")
    {
        return new WindowsWindow(width, height, name);
    }

} // namespace nwt
