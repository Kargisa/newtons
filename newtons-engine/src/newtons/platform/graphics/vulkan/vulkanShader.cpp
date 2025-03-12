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
			buffer = reinterpret_cast<uint32_t*>(File::readBinary((_vertPath), fileSize));
			break;
		case VulkanShader::ShaderType::FRAGMENT:
			buffer = reinterpret_cast<uint32_t*>(File::readBinary((_fragPath), fileSize));
			break;
		}

		*size = static_cast<uint32_t>(fileSize);
		return buffer;
	}

	VkShaderModule VulkanShader::createShaderModule(VkDevice device, ShaderType type) const {
		uint32_t* codeSize;
		uint32_t* code = loadShader(type, codeSize);

		VkShaderModuleCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = *codeSize;
		info.pCode = code;

		VkShaderModule module;
		if (vkCreateShaderModule(device, &info, nullptr, &module) != VK_SUCCESS) {
			delete[] code;
			std::runtime_error("could not create shader module!");
		}
		delete[] code;

		return module;
	}
} // namespace nwt