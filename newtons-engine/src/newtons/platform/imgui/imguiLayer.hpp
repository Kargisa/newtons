#pragma once

#include "newtons/pch.hpp"
#include "newtons/layer.hpp"

namespace nwt
{
    class ImguiLayer : public Layer {
        virtual void onAttach() override;
        virtual void onDetach() override;
        virtual void onEvent(const Event& event) override;
        virtual void update() override;
    };
} // namespace nwt
