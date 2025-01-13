#pragma once

#include <array>
#include "vec4.hpp"
#include <string>

namespace nwt{
    struct alignas(16) Mat4x4{
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;

        Mat4x4(Vec4 col0, Vec4 col1, Vec4 col2, Vec4 col3) {
            m00 = col0.x;
            m10 = col0.y;
            m20 = col0.z;
            m30 = col0.w;

            m01 = col1.x;
            m11 = col1.y;
            m21 = col1.z;
            m31 = col1.w;

            m02 = col2.x;
            m12 = col2.y;
            m22 = col2.z;
            m32 = col2.w;

            m03 = col3.x;
            m13 = col3.y;
            m23 = col3.z;
            m33 = col3.w;
        }

        float& getValue(char row, char col);
        float getValue(char row, char col) const;

        Vec4 getRow(char row) const;
        Vec4 getCol(char col) const;

        Mat4x4 operator*(const Mat4x4& other) const;
        
        float& operator[](char n);
        float operator[](char n) const;

        std::string toString() const;
    };


    inline float& Mat4x4::getValue(char row, char col){
        return (*this)[row + col * 4];
    }

    inline float Mat4x4::getValue(char row, char col) const{
        return (*this)[row + col * 4];
    }

    inline Vec4 Mat4x4::getRow(char row) const {
        return{
            (*this)[row + 0],
            (*this)[row + 4],
            (*this)[row + 8],
            (*this)[row + 12]
        };
    }

    inline Vec4 Mat4x4::getCol(char col) const {
        const char c = col * 4;
        return {
            (*this)[0 + c],
            (*this)[1 + c],
            (*this)[2 + c],
            (*this)[3 + c]
        };
    }

    inline Mat4x4 Mat4x4::operator*(const Mat4x4& other) const {
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

    inline float& Mat4x4::operator[](char n) {
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

    inline float Mat4x4::operator[](char n) const{
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
}