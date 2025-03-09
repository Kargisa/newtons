#include "vulkanShader.hpp"
#include "file.hpp"

namespace nwt {
	std::filesystem::path VulkanShader::_path = "../../../../assets/shaders/compiledShaders";

	uint32_t* VulkanShader::loadShader(ShaderType type, uint32_t* const size) const
	{
		uint32_t* buffer = nullptr;
		size_t fileSize = 0;
		
		switch (type)
		{
		case VulkanShader::ShaderType::VERTEX:
			buffer = reinterpret_cast<uint32_t*>(File::readBinary((_path / _vertPath), fileSize));
			break;
		case VulkanShader::ShaderType::FRAGMENT:
			buffer = reinterpret_cast<uint32_t*>(File::readBinary((_path / _fragPath), fileSize));
			break;
		}

		*size = static_cast<uint32_t>(fileSize); 
		return buffer;
	}
} // namespace nwt