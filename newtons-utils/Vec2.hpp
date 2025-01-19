#pragma once

#include <iostream>
#include "Mathf.hpp"
#include "Vec3.hpp"
#include "hash.hpp"
#include <string>

namespace nwt
{
	struct alignas(8) Vec2
	{
		float x, y;
		constexpr Vec2(float x, float y)
			: x(x), y(y) {}
		constexpr Vec2()
			: x(0), y(0) {}

		static constexpr Vec2 up();
		static constexpr Vec2 down();
		static constexpr Vec2 left();
		static constexpr Vec2 right();

		static constexpr Vec2 scale(const Vec2& a, const Vec2& b);
		static constexpr float dot(const Vec2& a, const Vec2& b);
		static float distance(const Vec2& a, const Vec2& b);
		static constexpr Vec2 lerp(const Vec2& a, const Vec2& b, float t);

		constexpr float sqrMagnitude() const;
		float magnitude() const;
		static Vec2 normalize(const Vec2& vec);
		Vec2 normalized();

		//static Vec2 rotate(const Vec2& v, float angle);

		constexpr bool operator==(const Vec2& other) const;
		constexpr bool operator!=(const Vec2& other) const;

		constexpr Vec2 operator+(const Vec2& other) const;
		constexpr Vec2 operator-(const Vec2& other) const;
		constexpr Vec2 operator-() const;
		constexpr Vec2 operator*(float scalar) const;
		friend constexpr Vec2 operator*(float scalar, const Vec2& v);
		constexpr Vec2 operator/(float scalar) const;

		constexpr Vec2& operator+=(const Vec2& other);
		constexpr Vec2& operator-=(const Vec2& other);
		constexpr Vec2& operator*=(float other);
		constexpr Vec2& operator/=(float other);

		std::string toString() const;
	};

	//
	// Static functions
	//

	inline constexpr Vec2 Vec2::up()
	{
		return Vec2(0, 1);
	}

	inline constexpr Vec2 Vec2::down()
	{
		return Vec2(0, -1);
	}

	inline constexpr Vec2 Vec2::left()
	{
		return Vec2(-1, 0);
	}

	inline constexpr Vec2 Vec2::right()
	{
		return Vec2(1, 0);
	}

	inline constexpr Vec2 Vec2::scale(const Vec2& a, const Vec2& b)
	{
		return Vec2(a.x * b.x, a.y * b.y);
	}

	inline constexpr float Vec2::dot(const Vec2& a, const Vec2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	inline float Vec2::distance(const Vec2& a, const Vec2& b)
	{
		return (b - a).magnitude();
	}

	inline constexpr Vec2 Vec2::lerp(const Vec2& a, const Vec2& b, float t)
	{
		return a + (b - a) * t;
	}

	//
	// Member functions
	//

	inline constexpr float Vec2::sqrMagnitude() const
	{
		return x * x + y * y;
	}

	inline float Vec2::magnitude() const
	{
		return Mathf::sqrt(x * x + y * y);
	}

	inline Vec2 Vec2::normalize(const Vec2& vec)
	{
		// TODO: Avoid division by zero in Debug mode
		float magnitude = vec.magnitude();
		return vec / magnitude;
	}

	inline Vec2 Vec2::normalized()
	{
		float magnitude = this->magnitude();

		return { x / magnitude, y / magnitude };
	}
	
	//constexpr Vec2 rotate(const Vec2& v, float angle) {
	//	return {v.x * Mathf::cos(angle) - v.y * Mathf::sin(angle), v.x * Mathf::sin(angle) + v.y * Mathf::cos(angle)};
	//}


	//
	// Operators
	//

	inline constexpr bool Vec2::operator==(const Vec2& other) const
	{
		return (x == other.x && y == other.y);
	}

	inline constexpr bool Vec2::operator!=(const Vec2& other) const
	{
		return !(*this == other);
	}

	inline constexpr Vec2 Vec2::operator+(const Vec2& other) const
	{
		return Vec2(x + other.x, y + other.y);
	}

	inline constexpr Vec2 Vec2::operator-(const Vec2& other) const
	{
		return Vec2(x - other.x, y - other.y);
	}

	inline constexpr Vec2 Vec2::operator-() const
	{
		return Vec2(-x, -y);
	}

	inline constexpr Vec2 Vec2::operator*(float scalar) const
	{
		return Vec2(x * scalar, y * scalar);
	}

	inline constexpr Vec2 operator*(float scalar, const Vec2& other)
	{
		return other * scalar;
	}

	inline constexpr Vec2 Vec2::operator/(float scalar) const
	{
		return Vec2(x / scalar, y / scalar);
	}

	inline constexpr Vec2& Vec2::operator+=(const Vec2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	inline constexpr Vec2& Vec2::operator-=(const Vec2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline constexpr Vec2& Vec2::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	inline constexpr Vec2& Vec2::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	inline std::string Vec2::toString() const{
		return ("(" + std::to_string(x) + "," + std::to_string(y) + ")");
	}

} // namespace nwt

namespace std {
	template<>
	struct hash<nwt::Vec2> {
		size_t operator()(nwt::Vec2 const& vec) const {
			size_t combinedHash = hash<float>()(vec.x);
			nwt::Hash::HashCombine(combinedHash, hash<float>()(vec.y));
			return combinedHash;
		}
	};
}