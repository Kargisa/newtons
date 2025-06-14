#include "application.hpp"

#include "newtons/platform/linux/linuxWindow.hpp"
#include "newtons/platform/windows/windowsWindow.hpp"
#include "newtons/logging/log.hpp"

namespace nwt {
    bool Application::_running = true;
    Window* Application::_window;
    GraphicsContext* Application::_graphicsContext;

    void Application::run() {
        initWindow();
        initGraphics();

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

        _window->setEventCallback([=](const Event& e) {
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

    void Application::initGraphics() {
        _graphicsContext = GraphicsContext::create(GraphicsAPI::VULKAN);
        _graphicsContext->initialize();
    }

    void Application::mainLoop()
    {
        while (_running) {
            auto start = std::chrono::high_resolution_clock::now();
            glfwPollEvents();
            _graphicsContext->drawFrame();
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = (end - start);
            // LOG_INFO(duration.count() * 1000.0d);
        }
    }

    void Application::cleanup()
    {
        delete _window;
        delete _graphicsContext;
    }

} // namespace nwt