#include "application.hpp"
#include <GLFW/glfw3.h>

namespace nwt{
    Application* Application::s_instance = nullptr;

    Application::Application()
    {
        if (s_instance == nullptr){
            s_instance = this;
        }
        else{
            return;
        }
    }

    Application::~Application()
    {
        if (s_instance == this){
            s_instance = nullptr;
        }
    }

    void Application::run(){
        initWindow();
        initVulkan();

        mainLoop();

        cleanup();
    }

    Window* const Application::getWindow()
    {
        return _window;
    }

    Application* Application::instace()
    {
        return s_instance;
    }

    void Application::initWindow()
    {
#ifdef NWT_LINUX
        _window = LinuxWindow::create(720, 405, "NEWTONS");
#elif NWT_WINDOWS
        _window = WindowsWindow::create(720, 405, "NEWTONS");
#endif
        _window->setEventCallback([&](const Event& e){
            if (e.getEventType() == Event::EventType::WindowClosed){
                _running = false;
            }
        });
    }

    void Application::initVulkan()
    {

    }

    void Application::mainLoop()
    {
        while (_running){
            glfwPollEvents();
        }
    }

    void Application::cleanup()
    {
        _window->destroy();
    }

} // namespace nwt