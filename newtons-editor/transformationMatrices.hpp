#pragma once
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES // helps with alignmet requirements
#include <glm/glm.hpp>

struct TransformationMatrices {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};