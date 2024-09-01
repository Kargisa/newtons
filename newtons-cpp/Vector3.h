#pragma once

#include <iostream>
#include "Math.h"

namespace Newtons
{
	struct Vector3
	{
	public:
		float x, y, z;
		Vector3(float x, float y, float z)
			: x(x), y(y), z(z) {}
		Vector3()
			: x(0), y(0), z(0) {}

		static Vector3 Up();
		static Vector3 Down();
		static Vector3 Left();
		static Vector3 Right();
		static Vector3 Forward();
		static Vector3 Backward();

		static Vector3 Scale(const Vector3& a, const Vector3& b);
		static float Dot(const Vector3& a, const Vector3& b);
		static Vector3 Cross(const Vector3& a, const Vector3& b);
		static float Distance(const Vector3& a, const Vector3& b);
		static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

		float SqrMagnitude() const;
		float Magnitude() const;
		Vector3 Normalized() const;
		void Normalize();

		bool operator==(const Vector3& other) const;
		bool operator!=(const Vector3& other) const;

		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-(const Vector3& other) const;
		Vector3 operator*(const float& scalar) const;
		friend Vector3 operator*(const float& scalar, const Vector3& v);
		Vector3 operator/(const float& scalar) const;
		friend Vector3 operator/(const float& scalar, const Vector3& v);

		Vector3& operator+=(const Vector3& other);
		Vector3& operator-=(const Vector3& other);
		Vector3& operator*=(const float& other);
		Vector3& operator/=(const float& other);

		friend std::ostream& operator<<(std::ostream& stream, const Vector3& vector);
	};

	//
	// Static functions
	//

	inline Vector3 Vector3::Up()
	{
		return Vector3(0, 1, 0);
	}

	inline Vector3 Vector3::Down()
	{
		return Vector3(0, -1, 0);
	}

	inline Vector3 Vector3::Left()
	{
		return Vector3(-1, 0, 0);
	}

	inline Vector3 Vector3::Right()
	{
		return Vector3(1, 0, 0);
	}

	inline Vector3 Vector3::Forward()
	{
		return Vector3(0, 0, 1);
	}

	inline Vector3 Vector3::Backward()
	{
		return Vector3(0, 0, -1);
	}

	inline Vector3 Vector3::Scale(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	inline float Vector3::Dot(const Vector3& a, const Vector3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
	{
		return Vector3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}

	inline float Vector3::Distance(const Vector3& a, const Vector3& b)
	{
		return (b - a).Magnitude();
	}

	inline Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t)
	{
		return a + (b - a) * t;
	}

	//
	// Member functions
	//

	inline float Vector3::SqrMagnitude() const
	{
		return x * x + y * y + z * z;
	}

	inline float Vector3::Magnitude() const
	{
		return Math::Sqrt(x * x + y * y + z * z);
	}

	inline Vector3 Vector3::Normalized() const
	{
		// TODO: Avoid division by zero in Debug mode
		float magnitude = Magnitude();
		return *this / magnitude;
	}

	inline void Vector3::Normalize()
	{
		float magnitude = Magnitude();
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}

	//
	// Operators
	//

	inline bool Vector3::operator==(const Vector3& other) const
	{
		return (x == other.x && y == other.y && z == other.z);
	}

	inline bool Vector3::operator!=(const Vector3& other) const
	{
		return !(*this == other);
	}

	inline Vector3 Vector3::operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	inline Vector3 Vector3::operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	inline Vector3 Vector3::operator*(const float& scalar) const
	{
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	inline Vector3 operator*(const float& scalar, const Vector3& v)
	{
		return v * scalar;
	}

	inline Vector3 operator/(const float& scalar, const Vector3& v)
	{
		return Vector3(scalar / v.x, scalar / v.y, scalar / v.z);
	}

	inline Vector3 Vector3::operator/(const float& scalar) const
	{
		return Vector3(x / scalar, y / scalar, z / scalar);
	}

	inline Vector3& Vector3::operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	inline Vector3& Vector3::operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	inline Vector3& Vector3::operator*=(const float& scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	inline Vector3& Vector3::operator/=(const float& scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}


	inline std::ostream& operator<<(std::ostream& stream, const Vector3& vector)
	{
		stream << vector.x << ", " << vector.y << ", " << vector.z;
		return stream;
	}
} // namespace Newtons