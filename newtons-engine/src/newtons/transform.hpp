#pragma once

#include "quaternion.hpp"
#include "vec3.hpp"
#include "mat4x4.hpp"

namespace nwt {
	class Transform {
	public:
		Vec3 position;
		Quaternion rotation;
		Vec3 scale;

	public:
		Transform(const Vec3& pos, const Quaternion& q, const Vec3& scale)
			: position(pos), rotation(q), scale(scale) {}

	public:
		Mat4x4 localToWorldMatrix() const;
		Mat4x4 worldToLocalMatrix() const;

		Vec3 forward() const;
		Vec3 up() const;
		Vec3 right() const;

		void setForward(const Vec3& vec);
		void setUp(const Vec3& vec);
		void setRight(const Vec3& vec);

	public:
		bool operator==(const Transform& other) const;
		bool operator!=(const Transform& other) const;
	};

	inline Mat4x4 Transform::localToWorldMatrix() const{
		Mat4x4 result = Mat4x4::rotate(rotation);
		result.m00 *= scale.x;
		result.m10 *= scale.x;
		result.m20 *= scale.x;

		result.m01 *= scale.y;
		result.m11 *= scale.y;
		result.m21 *= scale.y;

		result.m02 *= scale.z;
		result.m12 *= scale.z;
		result.m22 *= scale.z;


		result.setCol(3, position.x, position.y, position.z, 1);

		return result;
	}

	inline Mat4x4 Transform::worldToLocalMatrix() const{
		Quaternion rot = rotation.conjugated();

		Mat4x4 result = Mat4x4::rotate(rot);
		result.m00 /= scale.x;
		result.m10 /= scale.x;
		result.m20 /= scale.x;

		result.m01 /= scale.y;
		result.m11 /= scale.y;
		result.m21 /= scale.y;

		result.m02 /= scale.z;
		result.m12 /= scale.z;
		result.m22 /= scale.z;

		Vec3 pos = Quaternion::rotateVector(rot, -position);

		result.setCol(3, pos.x / scale.x, pos.y / scale.y, pos.z / scale.z, 1);
	
		return result;
	}

	inline Vec3 Transform::forward() const {
		return Quaternion::rotateVector(rotation, Vec3::forward());
	}

	inline Vec3 Transform::up() const {
		return Quaternion::rotateVector(rotation, Vec3::up());
	}

	inline Vec3 Transform::right() const {
		return Quaternion::rotateVector(rotation, Vec3::right());
	}

	inline void Transform::setForward(const Vec3& vec) {
		rotation = Quaternion::fromTo(Vec3::forward(), vec);
	}

	inline void Transform::setUp(const Vec3& vec) {
		rotation = Quaternion::fromTo(Vec3::up(), vec);
	}
	
	inline void Transform::setRight(const Vec3& vec) {
		rotation = Quaternion::fromTo(Vec3::right(), vec);
	}

	//
	// operators
	//

	bool Transform::operator==(const Transform& other) const {
		return position == other.position && rotation == other.rotation && scale == other.scale;
	}

	bool Transform::operator!=(const Transform& other) const {
		return !((*this) == other);
	}

}