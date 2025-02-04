#pragma once

#include "pch.hpp"
#include "application.hpp"
#include "window/window.hpp"
#include "newtons/platform/linux/linuxWindow.hpp"
#include "newtons/platform/windows/windowsWindow.hpp"
#include "newtons/renderer/vulkanRenderer.hpp"

namespace nwt{

class NWT_API Application{
    static Application* s_instance;

    bool _running = true;
    Window* _window;

public:
    Application();
    virtual ~Application();

public:
    void run();
    Window* const getWindow();

public:
    static Application* instace();

private:
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();
};

Application* createApp();

} // namespace nwt