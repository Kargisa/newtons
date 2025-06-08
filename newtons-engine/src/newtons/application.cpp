#include "application.hpp"

#include <GLFW/glfw3.h>
#include "newtons/platform/linux/linuxWindow.hpp"
#include "newtons/platform/windows/windowsWindow.hpp"
#include "newtons/logging/log.hpp"

namespace nwt {
    bool Application::_running = true;
    Window* Application::_window;
    GraphicsContext* Application::_graphicsContext;

    void Application::run() {
        initWindow();
        initVulkan();

        mainLoop();

        cleanup();
    }

    Window* Application::window() {
        return _window;
    }

    GraphicsContext* Application::graphcisContext() {
        return _graphicsContext;
    }

    void Application::initWindow()
    {
#ifdef NWT_LINUX
        _window = LinuxWindow::create("NEWTONS");
#elif NWT_WINDOWS
        _window = WindowsWindow::create("NEWTONS");
#endif
        _window->initialize(720, 405);

        _window->setEventCallback([&](const Event& e) {
            switch (e.getEventType())
            {
            case Event::EventType::WindowClosed: {
                _running = false;
                break;
            }
            case Event::EventType::WindowResized: {
                const WindowResizedEvent& resizeEvent = dynamic_cast<const WindowResizedEvent&>(e);
                break;
            }
            default:
                break;
            }

            });
    }

    void Application::initVulkan() {
        _graphicsContext = GraphicsContext::create(GraphicsAPI::VULKAN);
        _graphicsContext->initialze();
    }

    void Application::mainLoop()
    {
        while (_running) {
            glfwPollEvents();
            // static uint64_t i = 0;
            // LOG_INFO(i++);
            _graphicsContext->drawFrame();
        }
    }

    void Application::cleanup()
    {
        delete _window;
        delete _graphicsContext;
    }

} // namespace nwt