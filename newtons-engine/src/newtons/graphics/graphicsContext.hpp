#pragma once

#include "newtons/pch.hpp"
#include "newtons/layer.hpp"
#include "newtons/event/event.hpp"


namespace nwt {
    enum class GraphicsAPI {
        VULKAN,
    };


    class GraphicsContext {
    protected:
        GraphicsContext() = default;

    public:
        virtual ~GraphicsContext() = default;

        virtual void initialize() = 0;
        virtual void drawFrame() = 0;

        virtual void onEvent(const Event& event) = 0;
        virtual void* nativeContext() = 0;

        GraphicsAPI getAPI() const;

        static GraphicsContext* create(GraphicsAPI api);

    private:
        GraphicsAPI _api;
    };
} // namespace nwt
