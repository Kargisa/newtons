#pragma once

#include "pch.hpp"
#include "application.hpp"
#include "newtons/window/window.hpp"
#include "newtons/graphics/graphicsContext.hpp"

namespace nwt {

    class NWT_API Application {
        static bool _running;
        static Window* _window;
        static GraphicsContext* _graphicsContext;


    public:
        Application();
        virtual ~Application();

    public:
        static void init();
        static void run();
        static Window* getWindow();
        static GraphicsContext* getGraphcisContext();

    private:
        static void initWindow();
        static void initVulkan();
        static void mainLoop();
        static void cleanup();
    };

    Application* createApp();

} // namespace nwt