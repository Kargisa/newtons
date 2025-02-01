#pragma once
#include "hash.hpp"
#include "vec3.hpp"

#include <array>
#include <unordered_map>
#include <vulkan/vulkan.h>

namespace nwt{
    struct Vertex{
        Vec3 pos;
        Vec3 color;
        Vec2 texCoord;
        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();

        bool operator==(const Vertex& vert) const;
    };
}

namespace std {
	template<>
	struct hash<nwt::Vertex> {
		size_t operator()(nwt::Vertex const& vertex) const {
			size_t combinedHash = hash<nwt::Vec3>()(vertex.pos);
			nwt::Hash::HashCombine(combinedHash, hash<nwt::Vec3>()(vertex.color));
			nwt::Hash::HashCombine(combinedHash, hash<nwt::Vec2>()(vertex.texCoord));
			return combinedHash;
		}
	};
}