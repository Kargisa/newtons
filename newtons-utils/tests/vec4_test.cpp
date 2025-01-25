#include <catch2/catch_test_macros.hpp>
#include "vec4.hpp"

using namespace nwt;

TEST_CASE( "Vec4 sclae calculation", "[vec4]" ){
    REQUIRE(Vec4::scale({0, 0, 0, 0}, {2.4515f, 1.0f, 15, 1.2f}) == Vec4{0, 0, 0, 0});
    REQUIRE(Vec4::scale({2.4515f, 1.0f, 15, 1.2f}, {0, 0, 0, 0}) == Vec4{0, 0, 0, 0});

    REQUIRE(Vec4::scale({3, 1, 2.5f, 3.25f}, {8, 16.458f, 8, 1}) == Vec4{24, 16.458f, 20, 3.25f});
    REQUIRE(Vec4::scale({8, 16.458f, 8, 1}, {3, 1, 2.5f, 3.25f}) == Vec4{24, 16.458f, 20, 3.25f});

}

TEST_CASE( "Vec4 dot product calculation", "[vec4]" ){
    REQUIRE(Vec4::dot({0, 0, 0, 0}, {2.4515f, 1.0f, 15, 1.2f}) == 0);
    REQUIRE(Vec4::dot({2.4515f, 1.0f, 15, 1.2f}, {0, 0, 0, 0}) == 0);

    REQUIRE(Vec4::dot({2, 1, 3, 5}, {2, 1, 3, 5}) == 39);
    REQUIRE(Vec4::dot({1.54f, 8, 18.424f, 0}, {7, 13.87f, 11.11f, 0}) == 326.43064f);
}

TEST_CASE( "Vec4 distance calculation", "[vec4]" ){
    REQUIRE(Vec4::distance({0, 0, 0, 0}, {2.8f, 0, 0, 0}) == 2.8f);
    REQUIRE(Vec4::distance({-2.8f, 0, 0, 0}, {0, 0, 0, 0}) == 2.8f);
    
    REQUIRE(Vec4::distance({-2.8f, 4.2f, 10, 0}, {8, 0.2f, 1.25f, 0}) == 14.463834f);
}

TEST_CASE( "Vec4 lerp calculation", "[vec3]" ){
    REQUIRE(Vec4::lerp({0, 0, 0, 0}, {1, 1, 1, 1}, 0.248f) == Vec4{0.248f, 0.248f, 0.248f, 0.248f});
    REQUIRE(Vec4::lerp({1, 1, 1, 1}, {0, 0, 0, 0}, 0.248f) == Vec4{0.752f, 0.752f, 0.752f, 0.752f});
    REQUIRE(Vec4::lerp({0, 0, 0, 0}, {-1, 2, 0, 0}, 0.248f) == Vec4{-0.248f, 0.496f, 0.0f, 0.0f});
}

TEST_CASE( "Vec4 square magnitude calculation", "[vec3]" ){
    REQUIRE(Vec4(1, 0, 0, 0).sqrMagnitude() == 1.0f);
    REQUIRE(Vec4(2, 2, 2, 2).sqrMagnitude() == 16.0f);
    REQUIRE(Vec4(-2, 1, -2, -1).sqrMagnitude() == 10.0f);
}

TEST_CASE( "Vec4 magnitude calculation", "[vec4]" ){
    REQUIRE(Vec4(1, 0, 0, 0).magnitude() == 1.0f);
    REQUIRE(Vec4(2, 2, 2, 2).magnitude() == 4.0f);
    REQUIRE(Vec4(-2, 1, -2, -1).magnitude() == Mathf::sqrt(10.0f));
}

TEST_CASE( "Vec4 normalization calculation", "[vec4]" ){
    REQUIRE(Vec4::normalize(Vec4(1, 0, 0, 0)) == Vec4(1, 0, 0, 0));
    REQUIRE(Vec4::normalize(Vec4(-2, 1, -2, -1)) == Vec4(-2, 1, -2, -1) / Mathf::sqrt(10.0f));
    // REQUIRE(Vec3::normalize(Vec3()) == Vec3());
}

TEST_CASE( "Vec4 normalized calculation", "[vec4]" ){
    REQUIRE(Vec4(1, 0, 0, 0).normalized() == Vec4(1, 0, 0, 0));
    REQUIRE(Vec4(-2, 1, -2, -1).normalized() == Vec4(-2, 1, -2, -1) / Mathf::sqrt(10.0f));
    // REQUIRE(Vec3::normalize(Vec3()) == Vec3());
}