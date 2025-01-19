#pragma once

#include "Mathf.hpp"
#include "Vec3.hpp"
#include "hash.hpp"

#include <iostream>
#include <string>

namespace nwt
{
	struct alignas(16) Vec4
	{
		float x, y, z, w;
		constexpr Vec4(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w) {}
		constexpr Vec4()
			: x(0), y(0), z(0), w(0) {}

		static constexpr Vec4 scale(const Vec4& a, const Vec4& b);
		static constexpr float dot(const Vec4& a, const Vec4& b);
		static float distance(const Vec4& a, const Vec4& b);
		static constexpr Vec4 lerp(const Vec4& a, const Vec4& b, float t);

		float constexpr sqrMagnitude() const;
		float magnitude() const;
		static Vec4 normalize(const Vec4& vec);
		Vec4 normalized();

		constexpr bool operator==(const Vec4& other) const;
		constexpr bool operator!=(const Vec4& other) const;

		constexpr Vec4 operator+(const Vec4& other) const;
		constexpr Vec4 operator-(const Vec4& other) const;
		constexpr Vec4 operator-() const;
		constexpr Vec4 operator*(float scalar) const;
		friend constexpr Vec4 operator*(float scalar, const Vec4& v);
		constexpr Vec4 operator/(float scalar) const;

		constexpr Vec4& operator+=(const Vec4& other);
		constexpr Vec4& operator-=(const Vec4& other);
		constexpr Vec4& operator*=(float other);
		constexpr Vec4& operator/=(float other);

		explicit constexpr operator Vec3() const;

		std::string toString() const;
	};

	//
	// Static functions
	//

	inline constexpr Vec4 Vec4::scale(const Vec4& a, const Vec4& b)
	{
		return Vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w *b.w);
	}

	inline constexpr float Vec4::dot(const Vec4& a, const Vec4& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	inline float Vec4::distance(const Vec4& a, const Vec4& b)
	{
		return (b - a).magnitude();
	}

	inline constexpr Vec4 Vec4::lerp(const Vec4& a, const Vec4& b, float t)
	{
		return a + (b - a) * t;
	}

	//
	// Member functions
	//

	inline constexpr float Vec4::sqrMagnitude() const
	{
		return x * x + y * y + z * z + w * w;
	}

	inline float Vec4::magnitude() const
	{
		return Mathf::sqrt(x * x + y * y + z * z + w * w);
	}

	inline Vec4 Vec4::normalize(const Vec4& vec)
	{
		// TODO: Avoid division by zero in Debug mode
		float magnitude = vec.magnitude();
		return vec / magnitude;
	}

	inline Vec4 Vec4::normalized()
	{
		float magnitude = this->magnitude();

		return {x / magnitude, y / magnitude, z / magnitude, w / magnitude};
	}

	//
	// Operators
	//

	inline constexpr bool Vec4::operator==(const Vec4& other) const
	{
		return (x == other.x && y == other.y && z == other.z && w == other.w);
	}

	inline constexpr bool Vec4::operator!=(const Vec4& other) const
	{
		return !(*this == other);
	}

	inline constexpr Vec4 Vec4::operator+(const Vec4& other) const
	{
		return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	inline constexpr Vec4 Vec4::operator-(const Vec4& other) const
	{
		return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	inline constexpr Vec4 Vec4::operator-() const
	{
		return Vec4(-x, -y, -z, -w);
	}

	inline constexpr Vec4 Vec4::operator*(float scalar) const
	{
		return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	inline constexpr Vec4 operator*(float scalar, const Vec4& v)
	{
		return v * scalar;
	}

	inline constexpr Vec4 Vec4::operator/(float scalar) const
	{
		return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	inline constexpr Vec4& Vec4::operator+=(const Vec4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	inline constexpr Vec4& Vec4::operator-=(const Vec4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	inline constexpr Vec4& Vec4::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	inline constexpr Vec4& Vec4::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	inline constexpr Vec4::operator Vec3() const {
		return { x, y, z };
	}

	inline std::string Vec4::toString() const {
		return ("(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," + std::to_string(w) + ")");
	}
} // namespace nwt

namespace std {
	template<>
	struct hash<nwt::Vec4> {
		size_t operator()(nwt::Vec4 const& vec) const {
			size_t combinedHash = hash<float>()(vec.x);
			nwt::Hash::HashCombine(combinedHash, hash<float>()(vec.y));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(vec.z));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(vec.w));
			return combinedHash;
		}
	};
}