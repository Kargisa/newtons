#include "linuxWindow.hpp"

namespace nwt
{
    LinuxWindow::LinuxWindow(const char* name) {
        _data.name = name;
    }

    LinuxWindow::~LinuxWindow()
    {
        glfwDestroyWindow(_window);
    }

    void LinuxWindow::initialize(int width, int height)
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        _window = glfwCreateWindow(width, height, _data.name, nullptr, nullptr);
        glfwSetWindowUserPointer(_window, &_data);

        createCallbacks();
    }

    void LinuxWindow::createCallbacks()
    {
        glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            WindowData data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

            WindowResizedEvent event(width, height);
            data.eventCallback(event);
            });

        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
            WindowData data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.eventCallback(WindowClosedEvent());
            });

        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
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

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double x, double y) {
            WindowData data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

            MouseMovedEvent event(static_cast<float>(x), static_cast<float>(y));
            data.eventCallback(event);
            });

        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
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

        glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
            WindowData data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

            MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            data.eventCallback(event);
            });
    }

    void LinuxWindow::setEventCallback(EventFunc func) {
        _data.eventCallback = func;
    }

    void LinuxWindow::pollEvenets() {
        glfwPollEvents();
    }

    void* LinuxWindow::nativeWindow() {
        return _window;
    }

    void LinuxWindow::framebufferSize(int* width, int* height) {
        glfwGetFramebufferSize(_window, width, height);
        while (*width == 0 || *height == 0) {
            glfwWaitEvents();
            glfwGetFramebufferSize(_window, width, height);
        }
    }

    void LinuxWindow::destroy() {

    }

    // ********************************
    // ************ Vulkan ************
    // ********************************

    void LinuxWindow::createVulkanSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface)
    {
        glfwCreateWindowSurface(instance, _window, allocator, surface);
    }

    const char** LinuxWindow::getVulkanExtensions(uint32_t* count) {
        return glfwGetRequiredInstanceExtensions(count);
    }

    LinuxWindow* LinuxWindow::create(const char* name = "NEWTONS")
    {
        return new LinuxWindow(name);
    }

} // namespace nwt
