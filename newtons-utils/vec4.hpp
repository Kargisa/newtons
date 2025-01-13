#pragma once

#include <iostream>
#include "Mathf.hpp"
#include "Vec3.hpp"
#include <string>

namespace nwt
{
	struct alignas(16) Vec4
	{
		float x, y, z, w;
		Vec4(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w) {}
		Vec4()
			: x(0), y(0), z(0), w(0) {}
		Vec4(const Vec4& other)
			: x(other.x), y(other.y), z(other.z), w(other.w) {}

		static Vec4 scale(const Vec4& a, const Vec4& b);
		static float dot(const Vec4& a, const Vec4& b);
		static float distance(const Vec4& a, const Vec4& b);
		static Vec4 lerp(const Vec4& a, const Vec4& b, float t);

		float sqrMagnitude() const;
		float magnitude() const;
		Vec4 normalized() const;
		void normalize();

		bool operator==(const Vec4& other) const;
		bool operator!=(const Vec4& other) const;

		Vec4 operator+(const Vec4& other) const;
		Vec4 operator-(const Vec4& other) const;
		Vec4 operator*(float scalar) const;
		friend Vec4 operator*(float scalar, const Vec4& v);
		Vec4 operator/(float scalar) const;

		Vec4& operator+=(const Vec4& other);
		Vec4& operator-=(const Vec4& other);
		Vec4& operator*=(float other);
		Vec4& operator/=(float other);

		explicit operator Vec3() const;

		std::string toString() const;
	};

	//
	// Static functions
	//

	inline Vec4 Vec4::scale(const Vec4& a, const Vec4& b)
	{
		return Vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w *b.w);
	}

	inline float Vec4::dot(const Vec4& a, const Vec4& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	inline float Vec4::distance(const Vec4& a, const Vec4& b)
	{
		return (b - a).magnitude();
	}

	inline Vec4 Vec4::lerp(const Vec4& a, const Vec4& b, float t)
	{
		return a + (b - a) * t;
	}

	//
	// Member functions
	//

	inline float Vec4::sqrMagnitude() const
	{
		return x * x + y * y + z * z + w * w;
	}

	inline float Vec4::magnitude() const
	{
		return Mathf::sqrt(x * x + y * y + z * z + w * w);
	}

	inline Vec4 Vec4::normalized() const
	{
		// TODO: Avoid division by zero in Debug mode
		float magnitude = this->magnitude();
		return *this / magnitude;
	}

	inline void Vec4::normalize()
	{
		float magnitude = this->magnitude();
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
		w /= magnitude;
	}

	//
	// Operators
	//

	inline bool Vec4::operator==(const Vec4& other) const
	{
		return (x == other.x && y == other.y && z == other.z && w == other.w);
	}

	inline bool Vec4::operator!=(const Vec4& other) const
	{
		return !(*this == other);
	}

	inline Vec4 Vec4::operator+(const Vec4& other) const
	{
		return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	inline Vec4 Vec4::operator-(const Vec4& other) const
	{
		return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	inline Vec4 Vec4::operator*(float scalar) const
	{
		return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	inline Vec4 operator*(float scalar, const Vec4& v)
	{
		return v * scalar;
	}

	inline Vec4 Vec4::operator/(float scalar) const
	{
		return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	inline Vec4& Vec4::operator+=(const Vec4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	inline Vec4& Vec4::operator-=(const Vec4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	inline Vec4& Vec4::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	inline Vec4& Vec4::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	inline Vec4::operator Vec3() const {
		return { x, y, z };
	}

	inline std::string Vec4::toString() const {
		return ("(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," + std::to_string(w) + ")");
	}
} // namespace Newtons