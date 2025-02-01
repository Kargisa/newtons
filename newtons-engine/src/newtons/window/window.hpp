#pragma once

#include <GLFW/glfw3.h>

namespace nwt{
class Window{
    GLFWwindow* _window;

    static const int WIDTH = 720;
    static const int HEIGHT = 405;

public:
    Window(int width, int height, const char* name);
    ~Window();

public:
    
};
} // namespace nwt