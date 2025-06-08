#pragma once

#include "newtons/pch.hpp"


namespace nwt {
    enum class GraphicsAPI {
        VULKAN,
    };


    class GraphicsContext {
    protected:
        GraphicsContext() = default;

    public:
        inline virtual ~GraphicsContext() {}

        virtual void initialze() = 0;
        virtual void* getNativeContext() = 0;

        virtual void drawFrame() = 0;

        GraphicsAPI getAPI() const;

        static GraphicsContext* create(GraphicsAPI api);

    private:
        GraphicsAPI _api;
    };
} // namespace nwt
