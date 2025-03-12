#pragma once

#include "newtons/pch.hpp"

#include <filesystem>

namespace nwt {
    class Shader {
    public:
        Shader() = delete;

        static Shader* create(const std::filesystem::path& path);
        static Shader* find(const std::string& name);
        static bool setRootFolder(const std::filesystem::path& path);

    private:
        static std::unordered_map<std::string, Shader> _shaders;
        static std::filesystem::path _root;
    }
} // namespace nwt
