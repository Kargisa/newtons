#pragma once

#include "newtons/event/event.hpp"

namespace nwt
{
    class Layer {
    public:
        virtual ~Layer() = default;

        virtual void onAttach() = 0;
        virtual void onDetach() = 0;
        virtual void onEvent(const Event& event) = 0;
        virtual void update() = 0;
    };
} // namespace nwt
