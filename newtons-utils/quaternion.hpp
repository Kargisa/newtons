#pragma once

#include "vec3.hpp"

#include <string>

namespace nwt {
	/// <summary>
	/// The Quaternion is assumed to be a unit Quaternion
	/// </summary>
	struct alignas(16) Quaternion {
		float w, x, y, z;

		constexpr Quaternion(float w, float x, float y, float z)
			: w(w), x(x), y(y), z(z) {}
		constexpr Quaternion(float w, const Vec3& im)
			: w(w), x(im.x), y(im.y), z(im.z) {}

		static constexpr Quaternion identity();
		bool isNormalized() const;
		Quaternion normalized() const;

		float magnitude() const;

		static constexpr Quaternion conjugate(const Quaternion& q);
		constexpr Quaternion conjugated() const;

		static constexpr Vec3 rotateVector(const Quaternion& q, const Vec3& v);
		static Quaternion fromEuler(float x, float y, float z);
		static Quaternion fromTo(const Vec3& from, const Vec3& to);

		constexpr bool operator==(const Quaternion& q) const;
		constexpr bool operator!=(const Quaternion& q) const;

		constexpr Quaternion operator*(const Quaternion& q) const;
		constexpr Quaternion operator*(float scalar) const;
		friend constexpr Quaternion operator*(float scalar, const Quaternion& v);
		constexpr Quaternion operator/(float scalar) const;

		std::string toString() const;
	};

	inline constexpr Quaternion Quaternion::identity() {
		return { 1, 0, 0, 0 };
	}

	inline bool Quaternion::isNormalized() const {
		return Mathf::inEpsilon(Mathf::sqrt(x * x + y * y + z * z + w * w) - 1.0f);
	}

	inline Quaternion Quaternion::normalized() const {
		float magnitude = this->magnitude();

		return Quaternion{ x / magnitude, y / magnitude, z / magnitude, w / magnitude };
	}

	inline float Quaternion::magnitude() const {
		return Mathf::sqrt(x * x + y * y + z * z + w * w);
	}

	

	inline constexpr Quaternion Quaternion::conjugate(const Quaternion& q) {
		return { q.w, -q.x, -q.y, -q.z };
	}

	inline constexpr Quaternion Quaternion::conjugated() const{
		return { w, -x, -y, -z };
	}

	inline constexpr Vec3 Quaternion::rotateVector(const nwt::Quaternion& q, const Vec3& v) {
		Vec3 i{ q.x, q.y, q.z };

		return 2.0f * (q.x * v.x + q.y * v.y + q.z * v.z) * i +
			(q.w * q.w - i.sqrMagnitude()) * v +
			2.0f * q.w * Vec3(i.y * v.z - i.z * v.y, i.z * v.x - i.x * v.z, i.x * v.y - i.y * v.x);
	}

	// 3-2-1 (Z-Y-X)
	inline Quaternion Quaternion::fromEuler(float x, float y, float z){
		float cx = Mathf::cos(x * 0.5f);
		float sx = Mathf::sin(x * 0.5f);
		
		float cy = Mathf::cos(y * 0.5f);
		float sy = Mathf::sin(y * 0.5f);
		
		float cz = Mathf::cos(z * 0.5f);
		float sz = Mathf::sin(z * 0.5f);

		return{
			cx * cy * cz + sx * sy * sz,
			sx * cy * cz - cx * sy * sz,
			cx * sy * cz + sx * cy * sz,
			cx * cy * sz - sx * sy * cz
		};
	}

	inline Quaternion Quaternion::fromTo(const Vec3& from, const Vec3& to) {
		Vec3 dir = Vec3::cross(from, to);
		float real = Mathf::sqrt(from.sqrMagnitude() * to.sqrMagnitude() * Vec3::dot(from, to));
		return Quaternion{real, dir}.normalized();
	}

	//
	// Operators
	//

	inline constexpr bool Quaternion::operator==(const Quaternion& q) const{
		return Mathf::inEpsilon(w - q.w) && Mathf::inEpsilon(x - q.x) && Mathf::inEpsilon(y - q.y) && Mathf::inEpsilon(z - q.z);
	}

	inline constexpr bool Quaternion::operator!=(const Quaternion& q) const{
		return !((*this) == q);
	}

	inline constexpr Quaternion Quaternion::operator*(const Quaternion& q) const {
		Vec3 v1 = { x, y, z };
		Vec3 v2 = { q.x, q.y, q.z };
		
		float r = w * q.w - Vec3::dot(v1, v2);
		Vec3 i = (w * v2) + (q.w * v1) + Vec3::cross(v1, v2);

		return { r, i };
	}

	inline constexpr Quaternion Quaternion::operator*(float scalar) const
	{
		return { w * scalar, x * scalar, y * scalar, z * scalar };
	}

	inline constexpr Quaternion operator*(float scalar, const Quaternion& q)
	{
		return q * scalar;
	}

	inline constexpr Quaternion Quaternion::operator/(float scalar) const {
		return { w / scalar, x / scalar, y / scalar, z / scalar };
	}


	inline std::string Quaternion::toString() const {
		return ("(" + std::to_string(w) + "," + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")");
	}

} // namespace nwt

namespace std {
	template<>
	struct hash<nwt::Quaternion> {
		size_t operator()(nwt::Quaternion const& q) const {
			size_t combinedHash = hash<float>()(q.w);
			nwt::Hash::HashCombine(combinedHash, hash<float>()(q.x));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(q.y));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(q.z));
			return combinedHash;
		}
	};
}