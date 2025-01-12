#pragma once

#include <iostream>
#include "Mathf.hpp"
#include "Vec3.hpp"

namespace nwt
{
	struct alignas(8) Vec2
	{
		float x, y;
		Vec2(float x, float y)
			: x(x), y(y) {}
		Vec2()
			: x(0), y(0) {}

		static Vec2 up();
		static Vec2 down();
		static Vec2 left();
		static Vec2 right();

		static Vec2 scale(const Vec2& a, const Vec2& b);
		static float dot(const Vec2& a, const Vec2& b);
		static float distance(const Vec2& a, const Vec2& b);
		static Vec2 lerp(const Vec2& a, const Vec2& b, float t);

		float sqrMagnitude() const;
		float magnitude() const;
		Vec2 normalized() const;
		void normalize();

		bool operator==(const Vec2& other) const;
		bool operator!=(const Vec2& other) const;

		Vec2 operator+(const Vec2& other) const;
		Vec2 operator-(const Vec2& other) const;
		Vec2 operator*(float scalar) const;
		friend Vec2 operator*(float scalar, const Vec2& v);
		Vec2 operator/(float scalar) const;

		Vec2& operator+=(const Vec2& other);
		Vec2& operator-=(const Vec2& other);
		Vec2& operator*=(float other);
		Vec2& operator/=(float other);

		const char* toString() const;
	};

	//
	// Static functions
	//

	inline Vec2 Vec2::up()
	{
		return Vec2(0, 1);
	}

	inline Vec2 Vec2::down()
	{
		return Vec2(0, -1);
	}

	inline Vec2 Vec2::left()
	{
		return Vec2(-1, 0);
	}

	inline Vec2 Vec2::right()
	{
		return Vec2(1, 0);
	}

	inline Vec2 Vec2::scale(const Vec2& a, const Vec2& b)
	{
		return Vec2(a.x * b.x, a.y * b.y);
	}

	inline float Vec2::dot(const Vec2& a, const Vec2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	inline float Vec2::distance(const Vec2& a, const Vec2& b)
	{
		return (b - a).magnitude();
	}

	inline Vec2 Vec2::lerp(const Vec2& a, const Vec2& b, float t)
	{
		return a + (b - a) * t;
	}

	//
	// Member functions
	//

	inline float Vec2::sqrMagnitude() const
	{
		return x * x + y * y;
	}

	inline float Vec2::magnitude() const
	{
		return Mathf::sqrt(x * x + y * y);
	}

	inline Vec2 Vec2::normalized() const
	{
		// TODO: Avoid division by zero in Debug mode
		float magnitude = this->magnitude();
		return Vec2(x / magnitude, y / magnitude);
	}

	inline void Vec2::normalize()
	{
		float magnitude = this->magnitude();
		x /= magnitude;
		y /= magnitude;
	}

	//
	// Operators
	//

	inline bool Vec2::operator==(const Vec2& other) const
	{
		return (x == other.x && y == other.y);
	}

	inline bool Vec2::operator!=(const Vec2& other) const
	{
		return !(*this == other);
	}

	inline Vec2 Vec2::operator+(const Vec2& other) const
	{
		return Vec2(x + other.x, y + other.y);
	}

	inline Vec2 Vec2::operator-(const Vec2& other) const
	{
		return Vec2(x - other.x, y - other.y);
	}

	inline Vec2 Vec2::operator*(float scalar) const
	{
		return Vec2(x * scalar, y * scalar);
	}

	inline Vec2 operator*(float scalar, const Vec2& other)
	{
		return other * scalar;
	}

	inline Vec2 Vec2::operator/(float scalar) const
	{
		return Vec2(x / scalar, y / scalar);
	}

	inline Vec2& Vec2::operator+=(const Vec2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	inline Vec2& Vec2::operator-=(const Vec2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline Vec2& Vec2::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	inline Vec2& Vec2::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	inline const char* Vec2::toString() const{
		return ("(" + std::to_string(x) + "," + std::to_string(y) + ")").c_str();
	}

} // namespace Newtons