#include "window.hpp"

namespace nwt{
Window::Window(int width, int height, const char* name = "NEWTONS"){
    glfwInit();

    _window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    glfwSetWindowUserPointer(_window, this);
    //glfwSetKeyCallback
}

Window::~Window() {
    
}

} // namespace nwt
