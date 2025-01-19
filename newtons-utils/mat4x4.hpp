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

		Mat4x4() {}

		static constexpr Mat4x4 identity();
		static constexpr Mat4x4 rotate(const Quaternion& q);

		constexpr float& getValue(char row, char col);
		constexpr float getValue(char row, char col) const;

		constexpr Vec4 getRow(char row) const;
		constexpr Vec4 getCol(char col) const;


		constexpr Mat4x4 operator*(const Mat4x4& other) const;
		constexpr Vec4 operator*(const Vec4& vec) const;

		constexpr float& operator[](char n);
		constexpr float operator[](char n) const;

		constexpr std::string toString() const;
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

		Mat4x4 result;
		result.m00 = 1 - (num5 + num6);
		result.m10 = num7 + num12;
		result.m20 = num8 - num11;
		result.m30 = 0;
		result.m01 = num7 - num12;
		result.m11 = 1 - (num4 + num6);
		result.m21 = num9 + num10;
		result.m31 = 0;
		result.m02 = num8 + num11;
		result.m12 = num9 - num10;
		result.m22 = 1 - (num4 + num5);
		result.m32 = 0;
		result.m03 = 0;
		result.m13 = 0;
		result.m23 = 0;
		result.m33 = 1;

		return result;
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
		const char c = col * 4;
		return {
			(*this)[0 + c],
			(*this)[1 + c],
			(*this)[2 + c],
			(*this)[3 + c]
		};
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