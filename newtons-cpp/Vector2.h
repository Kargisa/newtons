#pragma once

#include <iostream>
#include "Math.h"

namespace Newtons
{
	struct Vector2
	{
	public:
		float x, y;
		Vector2(float x, float y)
			: x(x), y(y) {}
		Vector2()
			: x(0), y(0) {}

		static Vector2 Up();
		static Vector2 Down();
		static Vector2 Left();
		static Vector2 Right();

		static Vector2 Scale(const Vector2& a, const Vector2& b);
		static float Dot(const Vector2& a, const Vector2& b);
		static float Distance(const Vector2& a, const Vector2& b);
		static Vector2 Lerp(const Vector2& a, const Vector2& b, float t);

		float SqrMagnitude() const;
		float Magnitude() const;
		Vector2 Normalized() const;
		void Normalize();

		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;

		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		Vector2 operator*(const float& scalar) const;
		friend Vector2 operator*(const float& scalar, const Vector2& v);
		Vector2 operator/(const float& scalar) const;
		friend Vector2 operator/(const float& scalar, const Vector2& v);

		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);
		Vector2& operator*=(const float& other);
		Vector2& operator/=(const float& other);

		friend std::ostream& operator<<(std::ostream& stream, const Vector2& vector);
	};

	//
	// Static functions
	//

	inline Vector2 Vector2::Up()
	{
		return Vector2(0, 1);
	}

	inline Vector2 Vector2::Down()
	{
		return Vector2(0, -1);
	}

	inline Vector2 Vector2::Left()
	{
		return Vector2(-1, 0);
	}

	inline Vector2 Vector2::Right()
	{
		return Vector2(1, 0);
	}

	inline Vector2 Vector2::Scale(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x * b.x, a.y * b.y);
	}

	inline float Vector2::Dot(const Vector2& a, const Vector2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	inline float Vector2::Distance(const Vector2& a, const Vector2& b)
	{
		return (b - a).Magnitude();
	}

	inline Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float t)
	{
		return a + (b - a) * t;
	}

	//
	// Member functions
	//

	inline float Vector2::SqrMagnitude() const
	{
		return x * x + y * y;
	}

	inline float Vector2::Magnitude() const
	{
		return Math::Sqrt(x * x + y * y);
	}

	inline Vector2 Vector2::Normalized() const
	{
		// TODO: Avoid division by zero in Debug mode
		float magnitude = Magnitude();
		return Vector2(x / magnitude, y / magnitude);
	}

	inline void Vector2::Normalize()
	{
		float magnitude = Magnitude();
		x /= magnitude;
		y /= magnitude;
	}

	//
	// Operators
	//

	inline bool Vector2::operator==(const Vector2& other) const
	{
		return (x == other.x && y == other.y);
	}

	inline bool Vector2::operator!=(const Vector2& other) const
	{
		return !(*this == other);
	}

	inline Vector2 Vector2::operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	inline Vector2 Vector2::operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	inline Vector2 Vector2::operator*(const float& scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	inline Vector2 operator*(const float& scalar, const Vector2& other)
	{
		return other * scalar;
	}

	inline Vector2 Vector2::operator/(const float& scalar) const
	{
		return Vector2(x / scalar, y / scalar);
	}

	inline Vector2 operator/(const float& scalar, const Vector2& v)
	{
		return Vector2(scalar / v.x, scalar / v.y);
	}

	inline Vector2& Vector2::operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	inline Vector2& Vector2::operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline Vector2& Vector2::operator*=(const float& scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	inline Vector2& Vector2::operator/=(const float& scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	inline std::ostream& operator<<(std::ostream& stream, const Vector2& vector)
	{
		stream << vector.x << ", " << vector.y;
		return stream;
	}
} // namespace Newtons