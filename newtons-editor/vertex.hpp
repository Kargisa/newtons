#pragma once
#include <glm/glm.hpp>

#include "hash.hpp"

#include <array>
#include <unordered_map>
#include <vulkan/vulkan.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace nwt{
    struct Vertex{
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();

        bool operator==(const Vertex& vert) const;
    };
}

namespace std {
	template<>
	struct hash<nwt::Vertex> {
		size_t operator()(nwt::Vertex const& vertex) const {
			size_t combinedHash = hash<glm::vec3>()(vertex.pos);
			nwt::Hash::HashCombine(combinedHash, hash<glm::vec3>()(vertex.color));
			nwt::Hash::HashCombine(combinedHash, hash<glm::vec2>()(vertex.texCoord));
			return combinedHash;
		}
	};
}