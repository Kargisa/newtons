#pragma once

#include <iostream>
#include "Mathf.hpp"
#include "Vec2.hpp"
#include "hash.hpp"

namespace nwt
{
	struct alignas(16) Vec3
	{
		float x, y, z;
		constexpr Vec3(float x, float y, float z)
			: x(x), y(y), z(z) {}
		constexpr Vec3()
			: x(0), y(0), z(0) {}

		static constexpr Vec3 up();
		static constexpr Vec3 down();
		static constexpr Vec3 left();
		static constexpr Vec3 right();
		static constexpr Vec3 forward();
		static constexpr Vec3 backward();

		static constexpr Vec3 scale(const Vec3& a, const Vec3& b);
		static constexpr float dot(const Vec3& a, const Vec3& b);
		static constexpr Vec3 cross(const Vec3& a, const Vec3& b);
		static float distance(const Vec3& a, const Vec3& b);
		static constexpr Vec3 lerp(const Vec3& a, const Vec3& b, float t);

		float constexpr sqrMagnitude() const;
		float magnitude() const;
		static Vec3 normalize(const Vec3& vec);
		Vec3 normalized();

		//constexpr Vec3 lookAt(const Vec3& eye, const Vec3& center);

		constexpr bool operator==(const Vec3& other) const;
		constexpr bool operator!=(const Vec3& other) const;

		constexpr Vec3 operator+(const Vec3& other) const;
		constexpr Vec3 operator-(const Vec3& other) const;
		constexpr Vec3 operator-() const;
		constexpr Vec3 operator*(float scalar) const;
		friend constexpr Vec3 operator*(float scalar, const Vec3& v);
		constexpr Vec3 operator/(float scalar) const;

		constexpr Vec3& operator+=(const Vec3& other);
		constexpr Vec3& operator-=(const Vec3& other);
		constexpr Vec3& operator*=(float other);
		constexpr Vec3& operator/=(float other);

		explicit constexpr operator Vec2() const;

		std::string toString() const;
	};

	//
	// Static functions
	//

	inline constexpr Vec3 Vec3::up()
	{
		return Vec3(0, 1, 0);
	}

	inline constexpr Vec3 Vec3::down()
	{
		return Vec3(0, -1, 0);
	}

	inline constexpr Vec3 Vec3::left()
	{
		return Vec3(-1, 0, 0);
	}

	inline constexpr Vec3 Vec3::right()
	{
		return Vec3(1, 0, 0);
	}

	inline constexpr Vec3 Vec3::forward()
	{
		return Vec3(0, 0, 1);
	}

	inline constexpr Vec3 Vec3::backward()
	{
		return Vec3(0, 0, -1);
	}

	inline constexpr Vec3 Vec3::scale(const Vec3& a, const Vec3& b)
	{
		return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	inline constexpr float Vec3::dot(const Vec3& a, const Vec3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline constexpr Vec3 Vec3::cross(const Vec3& lhs, const Vec3& rhs)
	{
		return Vec3(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		);
	}

	inline float Vec3::distance(const Vec3& a, const Vec3& b)
	{
		return (b - a).magnitude();
	}

	inline constexpr Vec3 Vec3::lerp(const Vec3& a, const Vec3& b, float t)
	{
		return a + (b - a) * t;
	}

	//
	// Member functions
	//

	inline constexpr float Vec3::sqrMagnitude() const
	{
		return x * x + y * y + z * z;
	}

	inline float Vec3::magnitude() const
	{
		return Mathf::sqrt(x * x + y * y + z * z);
	}

	inline Vec3 Vec3::normalize(const Vec3& vec)
	{
		// TODO: Avoid division by zero in Debug mode
		float magnitude = vec.magnitude();
		return vec / magnitude;
	}

	inline Vec3 Vec3::normalized()
	{
		float magnitude = this->magnitude();

		return { x / magnitude, y / magnitude, z / magnitude };
	}

	//
	// Operators
	//

	inline constexpr bool Vec3::operator==(const Vec3& other) const
	{
		return (x == other.x && y == other.y && z == other.z);
	}

	inline constexpr bool Vec3::operator!=(const Vec3& other) const
	{
		return !(*this == other);
	}

	inline constexpr Vec3 Vec3::operator+(const Vec3& other) const
	{
		return Vec3(x + other.x, y + other.y, z + other.z);
	}

	inline constexpr Vec3 Vec3::operator-(const Vec3& other) const
	{
		return Vec3(x - other.x, y - other.y, z - other.z);
	}

	inline constexpr Vec3 Vec3::operator-() const
	{
		return Vec3(-x, -y, -z);
	}

	inline constexpr Vec3 Vec3::operator*(float scalar) const
	{
		return Vec3(x * scalar, y * scalar, z * scalar);
	}

	inline constexpr Vec3 operator*(float scalar, const Vec3& v)
	{
		return v * scalar;
	}

	inline constexpr Vec3 Vec3::operator/(float scalar) const
	{
		return Vec3(x / scalar, y / scalar, z / scalar);
	}

	inline constexpr Vec3& Vec3::operator+=(const Vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	inline constexpr Vec3& Vec3::operator-=(const Vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	inline constexpr Vec3& Vec3::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	inline constexpr Vec3& Vec3::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	inline constexpr Vec3::operator Vec2() const {
		return {x, y};
	}

	inline std::string Vec3::toString() const{
		return ("(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")");
	}
} // namespace nwt

namespace std {
	template<>
	struct hash<nwt::Vec3> {
		size_t operator()(nwt::Vec3 const& vec) const {
			size_t combinedHash = hash<float>()(vec.x);
			nwt::Hash::HashCombine(combinedHash, hash<float>()(vec.y));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(vec.z));
			return combinedHash;
		}
	};
}