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

	inline Mat4x4 Transform::localToWorldMatrix() const{
		Mat4x4 result = Mat4x4::rotate(_rotation);
		result.m00 *= _scale.x;
		result.m10 *= _scale.x;
		result.m20 *= _scale.x;

		result.m01 *= _scale.y;
		result.m11 *= _scale.y;
		result.m21 *= _scale.y;

		result.m02 *= _scale.z;
		result.m12 *= _scale.z;
		result.m22 *= _scale.z;

		result.setCol(3, _position.x, _position.y, _position.z, 1);
	}

	inline Mat4x4 Transform::worldToLocalMatrix() const{
		Mat4x4 result = Mat4x4::rotate(_rotation.conjugated());
		result.m00 *= _scale.x;
		result.m10 *= _scale.x;
		result.m20 *= _scale.x;

		result.m01 *= _scale.y;
		result.m11 *= _scale.y;
		result.m21 *= _scale.y;

		result.m02 *= _scale.z;
		result.m12 *= _scale.z;
		result.m22 *= _scale.z;

		result.setCol(3, _position.x, _position.y, _position.z, 1);
	}
}