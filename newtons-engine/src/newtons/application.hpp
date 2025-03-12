#pragma once

#include "pch.hpp"
#include "application.hpp"
#include "newtons/window/window.hpp"
#include "newtons/graphics/graphicsContext.hpp"

namespace nwt {

    class NWT_API Application {
        static Application* s_instance;

        bool _running = true;
        Window* _window;
        GraphicsContext* _graphicsContext;


    public:
        Application();
        virtual ~Application();

    public:
        void init();
        void run();
        Window* getWindow();
        GraphicsContext* getGraphcisContext();

    public:
        static Application* instance();

    private:
        void initWindow();
        void initVulkan();
        void mainLoop();
        void cleanup();
    };

    Application* createApp();

} // namespace nwt