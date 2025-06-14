#pragma once

#include <stdio.h>
#include <chrono>

namespace nwt
{
#ifdef NWT_DEBUG

#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"
#define WHITE_COLOR "\033[37m"

    enum LogLevel {

    };

    inline void currentTime(int* hours, int* minutes, int* seconds) {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm* local_tm = std::localtime(&now_time);

        *hours = local_tm->tm_hour;
        *minutes = local_tm->tm_min;
        *seconds = local_tm->tm_sec;
    }

#define TIME_FORMAT(h, m, s) "[" << (h < 10 ? "0" : "") << h << ":" << (m < 10 ? "0" : "") << m << ":" << (s < 10 ? "0" : "") << s << "] "

#define LOG_INFO(x)                                                                         \
    do {                                                                                    \
        int hours = -1;                                                                     \
        int minutes = -1;                                                                   \
        int seconds = -1;                                                                   \
        currentTime(&hours, &minutes, &seconds);                                            \
        std::cout << WHITE_COLOR << TIME_FORMAT(hours, minutes, seconds) << x << "\n";      \
    } while (0);

#define LOG_WARN(x)                                                                         \
    do {                                                                                    \
        int hours = -1;                                                                     \
        int minutes = -1;                                                                   \
        int seconds = -1;                                                                   \
        currentTime(&hours, &minutes, &seconds);                                            \
        std::cout << YELLOW_COLOR << TIME_FORMAT(hours, minutes, seconds) << x << WHITE_COLOR << "\n"; \
    } while (0);

#define LOG_OK(x)                                                                           \
    do {                                                                                    \
        int hours = -1;                                                                     \
        int minutes = -1;                                                                   \
        int seconds = -1;                                                                   \
        currentTime(&hours, &minutes, &seconds);                                            \
        std::cout << GREEN_COLOR << TIME_FORMAT(hours, minutes, seconds) << x << WHITE_COLOR << "\n"; \
    } while (0);

#define LOG_FAIL(x)                                                                         \
    do {                                                                                    \
        int hours = -1;                                                                     \
        int minutes = -1;                                                                   \
        int seconds = -1;                                                                   \
        currentTime(&hours, &minutes, &seconds);                                            \
        std::cout << RED_COLOR << TIME_FORMAT(hours, minutes, seconds) << x << WHITE_COLOR << "\n"; \
    } while (0);

#define LOG_SPACE() std::cout << std::endl;

#else

#define LOG_INFO(x);
#define LOG_WARN(x);
#define LOG_OK(x);
#define LOG_FAIL(x);
#define LOG_SPACE();

#endif    
} // namespace nwt

