#include "application.hpp"

#include "newtons/platform/linux/linuxWindow.hpp"
#include "newtons/platform/windows/windowsWindow.hpp"
#include "newtons/logging/log.hpp"

namespace nwt {
    bool Application::_running = true;
    Window* Application::_window;
    GraphicsContext* Application::_graphicsContext;
    LayerStack Application::_layers;

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

    const LayerStack& Application::layers() {
        return _layers;
    }

    void Application::attachLayer(Layer* layer) {
        _layers.attach(layer);
    }

    void Application::detachLayer(Layer* layer) {
        _layers.detach(layer);
    }

    void Application::initWindow()
    {
#ifdef NWT_LINUX
        _window = LinuxWindow::create("NEWTONS");
#elif NWT_WINDOWS
        _window = WindowsWindow::create("NEWTONS");
#endif

        _window->initialize(800, 450);

        _window->setEventCallback([&](const Event& e) {
            if (e.getEventType() == Event::EventType::WindowClosed) {
                _running = false;
            }

            _graphicsContext->onEvent(e);

            for (auto ritr = _layers.rbegin(); ritr != _layers.rend(); ++ritr) {
                (*ritr)->onEvent(e);
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
            _window->pollEvenets();
            for (auto&& layer : _layers) {
                layer->update();
            }

            _graphicsContext->drawFrame();
        }
    }

    void Application::cleanup()
    {
        delete _window;
        delete _graphicsContext;
    }

} // namespace nwt