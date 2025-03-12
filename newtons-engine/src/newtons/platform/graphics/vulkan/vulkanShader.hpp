#pragma once

#include <vulkan/vulkan.h>
#include <filesystem>

#include "newtons/pch.hpp"

namespace nwt {
	class VulkanShader
	{
	public:
		enum class ShaderType {
			VERTEX,
			FRAGMENT
		};

	public:

		VulkanShader(const std::filesystem::path& vertPath, const std::filesystem::path& fragPath)
			: _vertPath(vertPath), _fragPath(fragPath) {
		}

		~VulkanShader() = default;

		uint32_t* loadShader(ShaderType type, uint32_t* const size) const;

		VkShaderModule createShaderModule(VkDevice device, ShaderType type) const;

	private:
		std::filesystem::path _vertPath;
		std::filesystem::path _fragPath;
	};


} // namespace nwt