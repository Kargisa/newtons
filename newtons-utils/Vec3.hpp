#pragma once

#include <iostream>
#include "Mathf.hpp"
#include "Vec2.hpp"

namespace nwt
{
	struct alignas(16) Vec3
	{
		float x, y, z;
		Vec3(float x, float y, float z)
			: x(x), y(y), z(z) {}
		Vec3()
			: x(0), y(0), z(0) {}
		Vec3(const Vec3& other)
			: x(other.x), y(other.y), z(other.z) {}

		static Vec3 up();
		static Vec3 down();
		static Vec3 left();
		static Vec3 right();
		static Vec3 forward();
		static Vec3 backward();

		static Vec3 scale(const Vec3& a, const Vec3& b);
		static float dot(const Vec3& a, const Vec3& b);
		static Vec3 cross(const Vec3& a, const Vec3& b);
		static float distance(const Vec3& a, const Vec3& b);
		static Vec3 lerp(const Vec3& a, const Vec3& b, float t);

		float sqrMagnitude() const;
		float magnitude() const;
		Vec3 normalized() const;
		void normalize();

		bool operator==(const Vec3& other) const;
		bool operator!=(const Vec3& other) const;

		Vec3 operator+(const Vec3& other) const;
		Vec3 operator-(const Vec3& other) const;
		Vec3 operator*(float scalar) const;
		friend Vec3 operator*(float scalar, const Vec3& v);
		Vec3 operator/(float scalar) const;

		Vec3& operator+=(const Vec3& other);
		Vec3& operator-=(const Vec3& other);
		Vec3& operator*=(float other);
		Vec3& operator/=(float other);

		explicit operator Vec2() const;

		const char* toString() const;
	};

	//
	// Static functions
	//

	inline Vec3 Vec3::up()
	{
		return Vec3(0, 1, 0);
	}

	inline Vec3 Vec3::down()
	{
		return Vec3(0, -1, 0);
	}

	inline Vec3 Vec3::left()
	{
		return Vec3(-1, 0, 0);
	}

	inline Vec3 Vec3::right()
	{
		return Vec3(1, 0, 0);
	}

	inline Vec3 Vec3::forward()
	{
		return Vec3(0, 0, 1);
	}

	inline Vec3 Vec3::backward()
	{
		return Vec3(0, 0, -1);
	}

	inline Vec3 Vec3::scale(const Vec3& a, const Vec3& b)
	{
		return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	inline float Vec3::dot(const Vec3& a, const Vec3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline Vec3 Vec3::cross(const Vec3& a, const Vec3& b)
	{
		return Vec3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}

	inline float Vec3::distance(const Vec3& a, const Vec3& b)
	{
		return (b - a).magnitude();
	}

	inline Vec3 Vec3::lerp(const Vec3& a, const Vec3& b, float t)
	{
		return a + (b - a) * t;
	}

	//
	// Member functions
	//

	inline float Vec3::sqrMagnitude() const
	{
		return x * x + y * y + z * z;
	}

	inline float Vec3::magnitude() const
	{
		return Mathf::sqrt(x * x + y * y + z * z);
	}

	inline Vec3 Vec3::normalized() const
	{
		// TODO: Avoid division by zero in Debug mode
		float magnitude = this->magnitude();
		return *this / magnitude;
	}

	inline void Vec3::normalize()
	{
		float magnitude = this->magnitude();
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}

	//
	// Operators
	//

	inline bool Vec3::operator==(const Vec3& other) const
	{
		return (x == other.x && y == other.y && z == other.z);
	}

	inline bool Vec3::operator!=(const Vec3& other) const
	{
		return !(*this == other);
	}

	inline Vec3 Vec3::operator+(const Vec3& other) const
	{
		return Vec3(x + other.x, y + other.y, z + other.z);
	}

	inline Vec3 Vec3::operator-(const Vec3& other) const
	{
		return Vec3(x - other.x, y - other.y, z - other.z);
	}

	inline Vec3 Vec3::operator*(float scalar) const
	{
		return Vec3(x * scalar, y * scalar, z * scalar);
	}

	inline Vec3 operator*(float scalar, const Vec3& v)
	{
		return v * scalar;
	}

	inline Vec3 Vec3::operator/(float scalar) const
	{
		return Vec3(x / scalar, y / scalar, z / scalar);
	}

	inline Vec3& Vec3::operator+=(const Vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	inline Vec3& Vec3::operator-=(const Vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	inline Vec3& Vec3::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	inline Vec3& Vec3::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	inline Vec3::operator Vec2() const {
		return {x, y};
	}

	inline const char* Vec3::toString() const{
		return ("(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")").c_str();
	}
} // namespace Newtons