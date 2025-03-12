#pragma once

#include <algorithm> 
#include <cctype>
#include <locale>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>

namespace nwt {
    inline void lTrim(std::string& str) {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));
    }

    inline void rTrim(std::string& str) {
        str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
            }).base(), str.end());
    }

    inline void trim(std::string& str) {
        lTrim(str);
        rTrim(str);
    }

    inline std::vector<std::stirng> split(const std::string& str, char del) {
        std::stringstream stream = str;

        std::string tmp;
        std::vector<std::string> result;
        while (std::getline(stream, tmp, del)) {
            result.emplace(tmp);
        }

        return result;
    }
} // namespace nwt
