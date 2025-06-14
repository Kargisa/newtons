#include "newtons.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_vulkan.h"

class Game : public nwt::Application {

};

nwt::Application* nwt::createApp() {
    Application* game = new Game;
    return game;
}