#pragma once

#include "newtons/pch.hpp"

#include <filesystem>

namespace nwt {
    class Shader {
    public:
        // Shader() = delete;
        virtual ~Shader() {}

        static Shader* create(const std::filesystem::path& vertPath, const std::filesystem::path& fragPath, const std::string& name);
        static Shader* find(const std::string& name);
    private:
        // static passFile(const std::filesystem::path& path);

        static std::unordered_map<std::string, Shader> _shaders;
    };
} // namespace nwt
