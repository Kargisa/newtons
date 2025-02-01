#include "window.hpp"

namespace nwt{
Window::Window(int width, int height, const char* name = "NEWTONS"){
    glfwInit();

    _window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    glfwSetWindowUserPointer(_window, this);
}

Window::~Window() {
    
}

} // namespace nwt
