#pragma once

#include "newtons/pch.hpp"
#include "vec3.hpp"
#include "hash.hpp"

#include <vulkan/vulkan.h>

namespace nwt {

	struct VulkanVertex {
		Vec3 pos;
		Vec3 color;
		Vec2 texCoord;
		static VkVertexInputBindingDescription bindingDescription();
		static std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions();
	};

} //namespace nwt

namespace std {
	template<>
	struct hash<nwt::VulkanVertex> {
		size_t operator()(nwt::VulkanVertex const& vertex) const {
			size_t combinedHash = hash<nwt::Vec3>()(vertex.pos);
			nwt::Hash::HashCombine(combinedHash, hash<nwt::Vec3>()(vertex.color));
			nwt::Hash::HashCombine(combinedHash, hash<nwt::Vec2>()(vertex.texCoord));
			return combinedHash;
		}
	};
}