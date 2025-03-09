#pragma once

#include <string>
#include <filesystem>
#include <fstream>

namespace nwt {
	class File {
    public:
		static char* readBinary(const std::filesystem::path& path, size_t& fileSize);
	};

	inline char* File::readBinary(const std::filesystem::path& path, size_t& fileSize) {
        std::ifstream file(path, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        fileSize = (size_t)file.tellg();
        char* buffer = new char[fileSize];

        file.seekg(0);
        file.read(buffer, fileSize);

        file.close();

        return buffer;
    }

} // namespace nwt