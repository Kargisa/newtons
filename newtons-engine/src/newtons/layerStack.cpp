#include "layerStack.hpp"

namespace nwt
{
    void LayerStack::attach(Layer* layer) {
        layer->onAttach();
        _layers.emplace_back(layer);
    }

    void LayerStack::detach(Layer* layer) {
        layer->onDetach();
        _layers.erase(std::find(_layers.begin(), _layers.end(), layer));
    }

    constexpr Layer*& LayerStack::operator[](size_t n) {
        return _layers[n];
    }

    std::vector<Layer*>::iterator LayerStack::begin() {
        return _layers.begin();
    }

    std::vector<Layer*>::iterator LayerStack::end() {
        return _layers.end();
    }

    std::vector<Layer*>::reverse_iterator LayerStack::rbegin() {
        return _layers.rbegin();
    }

    std::vector<Layer*>::reverse_iterator LayerStack::rend() {
        return _layers.rend();
    }

} // namespace nwt
