#pragma once

#include "vec4.hpp"
#include "hash.hpp"
#include "quaternion.hpp"

#include <array>
#include <string>

namespace nwt {
	struct alignas(16) Mat4x4 {
		float m00, m01, m02, m03; // switching cols with rows here may be a better memory layout,
		float m10, m11, m12, m13; // because cols are more frequently accessed.
		float m20, m21, m22, m23; //
		float m30, m31, m32, m33; //

		constexpr Mat4x4(const Vec4& col0, const Vec4& col1, const Vec4& col2, const Vec4& col3)
			:	m00(col0.x), m10(col0.y), m20(col0.z), m30(col0.w),
				m01(col1.x), m11(col1.y), m21(col1.z), m31(col1.w),
				m02(col2.x), m12(col2.y), m22(col2.z), m32(col2.w),
				m03(col3.x), m13(col3.y), m23(col3.z), m33(col3.w) {}

		constexpr Mat4x4(float m00, float m10, float m20, float m30,
						 float m01, float m11, float m21, float m31,
						 float m02, float m12, float m22, float m32,
						 float m03, float m13, float m23, float m33)
						 : m00(m00), m10(m10), m20(m20), m30(m30),
						   m01(m01), m11(m11), m21(m21), m31(m31),
						   m02(m02), m12(m12), m22(m22), m32(m32),
						   m03(m03), m13(m13), m23(m23), m33(m33) {}

		//Mat4x4()
		//	: m00(0), m10(0), m20(0), m30(0),
		//	m01(0), m11(0), m21(0), m31(0),
		//	m02(0), m12(0), m22(0), m32(0),
		//	m03(0), m13(0), m23(0), m33(0) {}

		Mat4x4() {}

		static constexpr Mat4x4 identity();
		static constexpr Mat4x4 rotate(const Quaternion& q);

		constexpr float& getValue(char row, char col);
		constexpr float getValue(char row, char col) const;

		constexpr Vec4 getRow(char row) const;
		constexpr Vec4 getCol(char col) const;

		void setRow(char row, float x, float y, float z, float w);
		void setRow(char row, const Vec4& value);

		void setCol(char col, float x, float y, float z, float w);
		void setCol(char col, const Vec4& value);

		static constexpr Mat4x4 ortho(float left, float right, float top, float bottom, float near, float far);
		static Mat4x4 perspective(float fov, float aspect, float near, float far);

		static Mat4x4 lookAt(const Vec3& pos, const Vec3& target);

		constexpr bool operator==(const Mat4x4& mat) const;
		constexpr bool operator!=(const Mat4x4& mat) const;

		constexpr Mat4x4 operator*(const Mat4x4& other) const;
		constexpr Vec4 operator*(const Vec4& vec) const;
		constexpr Mat4x4 operator*(float scalar) const;
		friend constexpr Mat4x4 operator*(float scalar, const Mat4x4& mat);

		constexpr float& operator[](char n);
		constexpr float operator[](char n) const;

		std::string toString() const;
	};

	inline constexpr Mat4x4 Mat4x4::identity() {
		return {
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,0,0,1}
		};
	}

	inline constexpr Mat4x4 Mat4x4::rotate(const Quaternion& q) {
		float num = q.x * 2;
		float num2 = q.y * 2;
		float num3 = q.z * 2;
		float num4 = q.x * num;
		float num5 = q.y * num2;
		float num6 = q.z * num3;
		float num7 = q.x * num2;
		float num8 = q.x * num3;
		float num9 = q.y * num3;
		float num10 = q.w * num;
		float num11 = q.w * num2;
		float num12 = q.w * num3;

		return Mat4x4{
			1 - (num5 + num6),
			num7 + num12,
			num8 - num11,
			0,

			num7 - num12,
			1 - (num4 + num6),
			num9 + num10,
			0,

			num8 + num11,
			num9 - num10,
			1 - (num4 + num5),
			0,

			0,
			0,
			0,
			1
		};
	}

	inline constexpr float& Mat4x4::getValue(char row, char col) {
		return (*this)[row + col * 4];
	}

	inline constexpr float Mat4x4::getValue(char row, char col) const {
		return (*this)[row + col * 4];
	}

	inline constexpr Vec4 Mat4x4::getRow(char row) const {
		return{
			(*this)[row + 0],
			(*this)[row + 4],
			(*this)[row + 8],
			(*this)[row + 12]
		};
	}

	inline constexpr Vec4 Mat4x4::getCol(char col) const {
		char c = col * 4;
		return {
			(*this)[0 + c],
			(*this)[1 + c],
			(*this)[2 + c],
			(*this)[3 + c]
		};
	}

	inline void Mat4x4::setRow(char row, float x, float y, float z, float w){
		(*this)[row + 0] = x;
		(*this)[row + 4] = y;
		(*this)[row + 8] = z;
		(*this)[row + 12] = w;
	}

	inline void Mat4x4::setRow(char row, const Vec4& value){
		(*this)[row + 0] = value.x;
		(*this)[row + 4] = value.y;
		(*this)[row + 8] = value.z;
		(*this)[row + 12] = value.w;
	}

	inline void Mat4x4::setCol(char col, float x, float y, float z, float w){
		const char c = col * 4;
		(*this)[0 + c] = x;
		(*this)[1 + c] = y;
		(*this)[2 + c] = z;
		(*this)[3 + c] = w;
	}

	inline void Mat4x4::setCol(char col, const Vec4& value){
		const char c = col * 4;
		(*this)[0 + c] = value.x;
		(*this)[1 + c] = value.y;
		(*this)[2 + c] = value.z;
		(*this)[3 + c] = value.w;
	}

	inline constexpr Mat4x4 Mat4x4::ortho(float left, float right, float bottom, float top, float near, float far) {
		float rml = right - left;
		float lmr = left - right;
		float tmb = top - bottom;
		float bmt = bottom - top;
		float fmn = far - near;

		return { 
			2.0f / rml, 0, 0, 0, 
			0, 2.0f / tmb, 0, 0, 
			0, 0, 1.0f / fmn, 0, 
			-(right + left) / rml, -(top + bottom) / tmb, -near / fmn, 1.0f 
		};
	}

	inline Mat4x4 Mat4x4::perspective(float fov, float aspect, float near, float far) {
		float tanHalfFOV = Mathf::tan(fov / 2.0f);
		
		Mat4x4 result;

		result[0] = 1.0f / (aspect * tanHalfFOV);
		result[4] = 1.0f / (tanHalfFOV);
		result[8] = far / (far - near);
		result[14] = 1.0f;
		result[11] = -(far * near) / (far - near);

		//Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
		//Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
		//Result[2][2] = zFar / (zFar - zNear);
		//Result[2][3] = static_cast<T>(1);
		//Result[3][2] = -(zFar * zNear) / (zFar - zNear);

		return result;
	}

	inline Mat4x4 Mat4x4::lookAt(const Vec3& pos, const Vec3& target) {
		Vec3 forward = (target - pos).normalized();
		Vec3 right = Vec3::cross(Vec3::up(), forward).normalized();
		Vec3 up = Vec3::cross(forward, right);

		Mat4x4 result = Mat4x4::identity();

		result.setCol(0, right.x, right.y, right.z, 0);
		result.setCol(1, up.x, up.y, up.z, 0);
		result.setCol(2, forward.x, forward.y, forward.z, 0);
		result.setCol(3, -Vec3::dot(right, pos), -Vec3::dot(up, pos), -Vec3::dot(forward, pos), 1.0f);
		//result.setCol(3, pos.x, pos.y, pos.z, 1.0f);

		return result;
	}


	//
	// Operators
	//

	inline constexpr bool Mat4x4::operator==(const Mat4x4& mat) const{
		return (Mathf::inEpsilon(m00 - mat.m00) && Mathf::inEpsilon(m10 - mat.m10) && Mathf::inEpsilon(m20 - mat.m20) && Mathf::inEpsilon(m30 - mat.m30) ||
			Mathf::inEpsilon(m01 - mat.m01) && Mathf::inEpsilon(m11 - mat.m11) && Mathf::inEpsilon(m21 - mat.m21) && Mathf::inEpsilon(m31 - mat.m31) ||
			Mathf::inEpsilon(m02 - mat.m02) && Mathf::inEpsilon(m12 - mat.m12) && Mathf::inEpsilon(m22 - mat.m22) && Mathf::inEpsilon(m32 - mat.m32) ||
			Mathf::inEpsilon(m03 - mat.m03) && Mathf::inEpsilon(m13 - mat.m13) && Mathf::inEpsilon(m23 - mat.m23) && Mathf::inEpsilon(m33 - mat.m33));
	}

	inline constexpr bool Mat4x4::operator!=(const Mat4x4& mat) const{
		return !((*this) == mat);
	}

	inline constexpr Mat4x4 Mat4x4::operator*(const Mat4x4& other) const {
		Vec4 r0 = getRow(0);
		Vec4 r1 = getRow(1);
		Vec4 r2 = getRow(2);
		Vec4 r3 = getRow(3);

		Vec4 c0 = other.getCol(0);
		Vec4 c1 = other.getCol(1);
		Vec4 c2 = other.getCol(2);
		Vec4 c3 = other.getCol(3);

		return{
			{Vec4::dot(r0, c0), Vec4::dot(r1, c0), Vec4::dot(r2, c0), Vec4::dot(r3, c0)},
			{Vec4::dot(r0, c1), Vec4::dot(r1, c1), Vec4::dot(r2, c1), Vec4::dot(r3, c1)},
			{Vec4::dot(r0, c2), Vec4::dot(r1, c2), Vec4::dot(r2, c2), Vec4::dot(r3, c2)},
			{Vec4::dot(r0, c3), Vec4::dot(r1, c3), Vec4::dot(r2, c3), Vec4::dot(r3, c3)},
		};
	}

	inline constexpr Vec4 Mat4x4::operator*(const Vec4& vec) const {
		Vec4 r0 = getRow(0);
		Vec4 r1 = getRow(1);
		Vec4 r2 = getRow(2);
		Vec4 r3 = getRow(3);

		return { Vec4::dot(r0, vec), Vec4::dot(r1, vec), Vec4::dot(r2, vec), Vec4::dot(r3, vec) };
	}

	inline constexpr float& Mat4x4::operator[](char n) {
		switch (n)
		{
		case 0:
			return m00;
		case 1:
			return m10;
		case 2:
			return m20;
		case 3:
			return m30;
		case 4:
			return m01;
		case 5:
			return m11;
		case 6:
			return m21;
		case 7:
			return m31;
		case 8:
			return m02;
		case 9:
			return m12;
		case 10:
			return m22;
		case 11:
			return m32;
		case 12:
			return m03;
		case 13:
			return m13;
		case 14:
			return m23;
		case 15:
			return m33;
		}

		// TODO: Error handling
	}

	inline constexpr float Mat4x4::operator[](char n) const {
		switch (n)
		{
		case 0:
			return m00;
		case 1:
			return m10;
		case 2:
			return m20;
		case 3:
			return m30;
		case 4:
			return m01;
		case 5:
			return m11;
		case 6:
			return m21;
		case 7:
			return m31;
		case 8:
			return m02;
		case 9:
			return m12;
		case 10:
			return m22;
		case 11:
			return m32;
		case 12:
			return m03;
		case 13:
			return m13;
		case 14:
			return m23;
		case 15:
			return m33;
		}

		// TODO: Error handling
	}


	inline constexpr Mat4x4 Mat4x4::operator*(float scalar) const{
		return {
			m00 * scalar,
			m10 * scalar,
			m20 * scalar,
			m30 * scalar,

			m01 * scalar,
			m11 * scalar,
			m21 * scalar,
			m31 * scalar,

			m02 * scalar,
			m12 * scalar,
			m22 * scalar,
			m32 * scalar,

			m03 * scalar,
			m13 * scalar,
			m23 * scalar,
			m33 * scalar
		};
	}

	inline constexpr Mat4x4 operator*(float scalar, const Mat4x4& mat){
		return mat * scalar;
	}

	inline std::string Mat4x4::toString() const {
		return (
			std::to_string(m00) + "," + std::to_string(m01) + "," + std::to_string(m02) + "," + std::to_string(m03) + "\n" +
			std::to_string(m10) + "," + std::to_string(m11) + "," + std::to_string(m12) + "," + std::to_string(m13) + "\n" +
			std::to_string(m20) + "," + std::to_string(m21) + "," + std::to_string(m22) + "," + std::to_string(m23) + "\n" +
			std::to_string(m30) + "," + std::to_string(m31) + "," + std::to_string(m32) + "," + std::to_string(m33)
			);
	}
} // namespace nwt

namespace std {
	template<>
	struct hash<nwt::Mat4x4> {
		size_t operator()(nwt::Mat4x4 const& mat) const {
			size_t combinedHash = hash<float>()(mat.m00);
			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m10));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m20));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m30));

			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m01));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m11));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m21));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m31));

			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m02));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m12));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m22));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m32));

			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m03));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m13));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m23));
			nwt::Hash::HashCombine(combinedHash, hash<float>()(mat.m33));
			return combinedHash;
		}
	};
}