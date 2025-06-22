#pragma once

#include "pch.hpp"
#include "application.hpp"
#include "newtons/window/window.hpp"
#include "newtons/graphics/graphicsContext.hpp"
#include "layer.hpp"
#include "layerStack.hpp"

namespace nwt {

    class Application {
        static bool _running;
        static Window* _window;
        static GraphicsContext* _graphicsContext;
        static LayerStack _layers;

    public:
        static void run();
        static Window* window();
        static GraphicsContext* graphcisContext();

        static const LayerStack& layers();

        static void attachLayer(Layer* layer);
        static void detachLayer(Layer* layer);

    private:
        static void initWindow();
        static void initGraphics();
        static void mainLoop();
        static void cleanup();
    };

    Application* createApp();

} // namespace nwt