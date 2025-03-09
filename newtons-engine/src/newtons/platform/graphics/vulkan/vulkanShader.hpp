#pragma once

#include "newtons/pch.hpp"
#include <filesystem>

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
			: _vertPath(vertPath), _fragPath(fragPath) {}

		VulkanShader(const VulkanShader& other)
			: _vertPath(other._vertPath), _fragPath(other._fragPath) {}

		~VulkanShader() = default;

		uint32_t* loadShader(ShaderType type, uint32_t* const size) const;

	private:
		std::filesystem::path _vertPath;
		std::filesystem::path _fragPath;
		
		static std::filesystem::path _path;
	};


} // namespace nwt