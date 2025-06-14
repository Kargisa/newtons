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
        static void run();
        static Window* window();
        static GraphicsContext* graphcisContext();

    private:
        static void initWindow();
        static void initGraphics();
        static void mainLoop();
        static void cleanup();
    };

    Application* createApp();

} // namespace nwt