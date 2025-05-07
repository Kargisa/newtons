#include "application.hpp"

#include <GLFW/glfw3.h>
#include "newtons/platform/linux/linuxWindow.hpp"
#include "newtons/platform/windows/windowsWindow.hpp"

namespace nwt {
    bool Application::_running = true;
    Window* Application::_window;
    GraphicsContext* Application::_graphicsContext;


    Application::Application() {

    }

    Application::~Application() {

    }

    void Application::init() {

    }

    void Application::run() {
        initWindow();
        initVulkan();

        mainLoop();

        cleanup();
    }

    Window* Application::getWindow() {
        return _window;
    }

    GraphicsContext* Application::getGraphcisContext() {
        return _graphicsContext;
    }

    void Application::initWindow()
    {
#ifdef NWT_LINUX
        _window = LinuxWindow::create(720, 405, "NEWTONS");
#elif NWT_WINDOWS
        _window = WindowsWindow::create(720, 405, "NEWTONS");
#endif
        _window->setEventCallback([&](const Event& e) {
            if (e.getEventType() == Event::EventType::WindowClosed) {
                _running = false;
            }
            });
    }

    void Application::initVulkan() {
        _graphicsContext = GraphicsContext::create(GraphicsAPI::VULKAN_API);
        _graphicsContext->init();
    }

    void Application::mainLoop()
    {
        while (_running) {
            glfwPollEvents();
        }
    }

    void Application::cleanup()
    {
        _window->destroy();
    }

} // namespace nwt