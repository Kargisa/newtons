#pragma once

#include <iostream>

#define GREEN_COLOR "\033[32m"
#define RED_COLOR "\033[31m"
#define WHITE_COLOR "\033[37m"

#ifdef DEBUG
    #define LOG_INFO(x) std::cout << x << "\n";
#else
    #define LOG_INFO(x)
#endif