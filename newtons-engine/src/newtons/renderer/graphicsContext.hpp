#pragma once

#include "newtons/pch.hpp"


namespace nwt {
    enum class GraphicsAPI {
        VULKAN_API,
    };


    class GraphicsContext {
    protected:
        inline GraphicsContext() = default;

    public:
        inline virtual ~GraphicsContext() {}

        virtual void init() = 0;
        virtual void* getNativeContext() = 0;

        static GraphicsContext* createContext(GraphicsAPI api);

    private:
        GraphicsAPI _api;
    };
} // namespace nwt
