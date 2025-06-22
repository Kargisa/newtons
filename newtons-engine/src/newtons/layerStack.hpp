#pragma once
#include "newtons/pch.hpp"
#include "layer.hpp"
#include "newtons/event/event.hpp"

namespace nwt
{
    class LayerStack {
        std::vector<Layer*> _layers;

    public:
        void attach(Layer* layer);
        void detach(Layer* layer);

        constexpr Layer*& operator[](size_t n);

        std::vector<Layer*>::iterator begin();
        std::vector<Layer*>::iterator end();

        std::vector<Layer*>::reverse_iterator rbegin();
        std::vector<Layer*>::reverse_iterator rend();
    };
} // namespace nwt
