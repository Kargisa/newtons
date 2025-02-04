#include "window.hpp"

namespace nwt{

void Window::setEventCallback(EventFunc func)
{
    _data.eventCallback = func;
}

int Window::getWidth() const
{
    return _data.width;
}

int Window::getHeight() const
{
    return _data.height;
}

void Window::destroy()
{
    delete this;
}

} // namespace nwt
