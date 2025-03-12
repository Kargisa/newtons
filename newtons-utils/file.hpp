#pragma once

#include <string>
#include <filesystem>
#include <fstream>

namespace nwt {
    class File {
    public:
        static char* readBinary(const std::filesystem::path& path, size_t& fileSize);
        static char* readText(const std::filesystem::path& path, size_t& fileSize);

        static bool pathExists(const std::filesystem::path& path);
        static bool isDirectory(const std::filesystem::path& path);
    };


    /// @brief reads the whole file as binary
    inline char* File::readBinary(const std::filesystem::path& path, size_t& fileSize) {
        std::ifstream file(path, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            fileSize = 0;
            return nullptr;
        }

        fileSize = (size_t)file.tellg();
        char* buffer = new char[fileSize];

        file.seekg(0);
        file.read(buffer, fileSize);

        file.close();

        return buffer;
    }

    /// @brief reads the whole file as text
    inline char* File::readText(const std::filesystem::path& path, size_t& fileSize) {
        std::ifstream file(path, std::ios::ate);

        if (!file.is_open()) {
            fileSize = 0;
            return nullptr;
        }

        fileSize = (size_t)file.tellg();
        char* buffer = new char[fileSize];

        file.seekg(0);
        file.read(buffer, fileSize);

        file.close();

        return buffer;
    }

    inline bool File::pathExists(const std::filesystem::path& path) {
        return std::filesystem::exists(path);
    }

    inline bool File::isDirectory(const std::filesystem::path& path) {
        return std::filesystem::is_directory(path);
    }
} // namespace nwt