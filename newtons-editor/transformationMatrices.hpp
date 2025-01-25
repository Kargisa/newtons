#pragma once
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES // helps with alignmet requirements
#include <glm/glm.hpp>
#include "mat4x4.hpp"

namespace nwt{
struct TransformationMatrices {
	Mat4x4 model;
	glm::mat4 view;
	glm::mat4 proj;
};
} // namespace nwt