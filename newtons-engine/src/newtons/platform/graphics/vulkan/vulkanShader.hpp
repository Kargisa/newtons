#pragma once

#include <vulkan/vulkan.h>
#include <filesystem>

#include "newtons/pch.hpp"
#include "newtons/graphics/shader.hpp"
#include "vulkanSwapchain.hpp"
#include "hash.hpp"

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

		bool operator==(const VulkanShader& other) const;
		bool operator!=(const VulkanShader& other) const;

		std::filesystem::path getVertPath() const;
		std::filesystem::path getFragPath() const;

		uint32_t* loadShader(ShaderType type, uint32_t* const size) const;


		VkShaderModule createShaderModule(VkDevice device, ShaderType type) const;
		void createGraphicsPipeline(VkDevice device, VkRenderPass renderPass, const VulkanSwapchain& swapChain);

	private:
		std::filesystem::path _vertPath;
		std::filesystem::path _fragPath;
		VkPipeline _pipeline;
	};
} // namespace nwt

namespace std {
	template<>
	struct hash<nwt::VulkanShader> {
		size_t operator()(nwt::VulkanShader const& shader) const {
			size_t combinedHash = hash<std::filesystem::path>()(shader.getVertPath());
			nwt::Hash::HashCombine(combinedHash, hash<std::filesystem::path>()(shader.getFragPath()));
			return combinedHash;
		}
	};
}