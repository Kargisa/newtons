#pragma once

#include <filesystem>

#include "fixedVector.hpp"

namespace nwt
{
    class FileReader {
    public:
        static FixedVector<char> readSpirV(std::filesystem::path path);
    };
} // namespace nwt
