#pragma once

#include "quaternion.hpp"
#include "vec3.hpp"
#include "mat4x4.hpp"

namespace nwt {
	class Transform {
	private:
		Vec3 _position;
		Quaternion _rotation;
		Vec3 _scale;

	public:
		Transform(const Vec3 pos, const Quaternion& q, const Vec3& scale)
			: _position(pos), _rotation(q), _scale(scale) {}

	public:
		Mat4x4 localToWorldMatrix() const;
		Mat4x4 worldToLocalMatrix() const;
	};
}