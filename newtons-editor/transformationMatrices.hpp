#pragma once
#include "mat4x4.hpp"

namespace nwt{
struct TransformationMatrices {
	Mat4x4 model;
	Mat4x4 view;
	Mat4x4 proj;
};
} // namespace nwt