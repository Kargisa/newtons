#include <catch2/catch_test_macros.hpp>
#include "mat4x4.hpp"

using namespace nwt;

TEST_CASE( "Mat4x4 identity check", "[mat4x4]" ){
    REQUIRE(Mat4x4::identity() == Mat4x4{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
}

TEST_CASE( "Mat4x4 rotation calculation", "[mat4x4]" ){
    REQUIRE(Mat4x4::rotate({Mathf::cos(78.1f * Mathf::DegToRad), Mathf::sin(78.1f * Mathf::DegToRad) * Vec3(1,0.5f,1).normalized()}) == Mat4x4{-0.0638665f, 0.6945768f, 0.7165781f, 0,
                                                                                                                                               0.1565164f, -0.7021864f, 0.6945768f, 0,
                                                                                                                                               0.9856083f, 0.1565164f, -0.0638665f, 0,
                                                                                                                                               0, 0, 0, 1});
}

TEST_CASE( "Mat4x4 x Mat4x4 multiplication calculation", "[mat4x4]" ){
    Mat4x4 lhs(2, 50, 10, 2, 1.5f, 2, 60, 2, 6, 6, 7, 2, 8, 48, 0.1f, 0);
    Mat4x4 rhs(8, 6, 84, 1, 94, 8.5f, 12, 54, 1.1f, 5, 23, 2, 0, 15, 1, 0.25);
    
    Mat4x4 result1(537, 644, 1348.1f, 196, 704.75f, 3621, 5299.4f, 229, 163.7f, 255, 516.2f, 58.2f, 30.5f, 48, 907.025f, 32);
    Mat4x4 result2(1011, 377, 1440, 642.5f, 266, 356, 1532, 230, 619.7f, 152, 739, 344.5f, 4576.11f, 456.5f, 1250.3f, 2600.2f);

    REQUIRE(lhs * rhs == result1);
    REQUIRE(rhs * lhs == result2);
}

TEST_CASE( "Mat4x4 x Vec4 multiplication calculation", "[mat4x4]" ){
    Mat4x4 mat1(2, 50, 10, 2, 1.5f, 2, 60, 2, 6, 6, 7, 2, 8, 48, 0.1f, 0);
    Vec4 vec(2.5f, 8, 0, 4.1f);

    REQUIRE(mat1 * vec == Vec4(49.8f, 337.8f, 505.41f, 21));

    Mat4x4 mat2(8, 6, 84, 1, 94, 8.5f, 12, 54, 1.1f, 5, 23, 2, 0, 15, 1, 0.25);

    REQUIRE(mat2 * vec == Vec4(772, 144.5f, 310.1f, 435.525f));
}

