#include "newtons/application.hpp"
#include "newtons/entryPoint.hpp"
#include "newtons/window/window.hpp"

#ifdef NWT_LINUX
#include "newtons/platform/linux/linuxWindow.hpp"
#elif NWT_WINDOWS
#include "newtons/platform/windows/windowsWindow.hpp"
#endif