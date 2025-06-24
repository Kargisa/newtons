#include "file.hpp"
#include <fstream>

namespace nwt
{
    FixedVector<char> FileReader::readSpirV(std::filesystem::path path)
    {
        std::ifstream file(path, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        FixedVector<char> buffer = FixedVector<char>(fileSize);

        file.seekg(0);
        file.read(buffer.data(), buffer.size());

        file.close();

        return buffer;
    }
} // namespace nwt