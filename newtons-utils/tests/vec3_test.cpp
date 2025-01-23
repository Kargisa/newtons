#include <catch2/catch_test_macros.hpp>
#include "vec3.hpp"

using namespace nwt;

TEST_CASE( "Vec3 direction checks", "[vec3]" ) {
    REQUIRE(Vec3::up() == Vec3{0, 1, 0});
    REQUIRE(Vec3::down() == Vec3{0, -1, 0});
    
    REQUIRE(Vec3::right() == Vec3{1, 0, 0});
    REQUIRE(Vec3::left() == Vec3{-1, 0, 0});

    REQUIRE(Vec3::forward() == Vec3{0, 0, 1});
    REQUIRE(Vec3::backward() == Vec3{0, 0, -1});
}

TEST_CASE( "Vec3 sclae calculation", "[vec3]" ){
    REQUIRE(Vec3::scale({0, 0, 0}, {2.4515f, 1.0f, 15}) == Vec3{0, 0, 0});
    REQUIRE(Vec3::scale({2.4515f, 1.0f, 15}, {0, 0, 0}) == Vec3{0, 0, 0});

    REQUIRE(Vec3::scale({3, 1, 2.5f}, {8, 16.458f, 8}) == Vec3{24, 16.458f, 20});
    REQUIRE(Vec3::scale({8, 16.458f, 8}, {3, 1, 2.5f}) == Vec3{24, 16.458f, 20});

}

TEST_CASE( "Vec3 dot product calculation", "[vec3]" ){
    REQUIRE(Vec3::dot({0, 0, 0}, {2.4515f, 1.0f, 15}) == 0);
    REQUIRE(Vec3::dot({2.4515f, 1.0f, 15}, {0, 0, 0}) == 0);

    REQUIRE(Vec3::dot({2, 1, 3}, {2, 1, 3}) == 14);
    REQUIRE(Vec3::dot({1.54f, 8, 18.424f}, {7, 13.87f, 11.11f}) == 326.43064f);
}

TEST_CASE( "Vec3 cross product calculation", "[vec3]" ){
    REQUIRE(Vec3::cross({1, 0, 0}, {0, 1, 0}) == Vec3{0, 0, 1});
    REQUIRE(Vec3::cross({0, 1, 0}, {1, 0, 0}) == Vec3{0, 0, -1});

    REQUIRE(Vec3::cross({0.4f, 3, 2.78f}, {0.4f, 3, 2.78f}) == Vec3{0, 0, 0});
    REQUIRE(Vec3::cross({9, 1.25f, 3.5f}, {1, 1, 1}) == Vec3{-2.25f, -5.5f, 7.75f});
}

TEST_CASE( "Vec3 distance calculation", "[vec3]" ){
    REQUIRE(Vec3::distance({0, 0, 0}, {2.8f, 0, 0}) == 2.8f);
    REQUIRE(Vec3::distance({-2.8f, 0, 0}, {0, 0, 0}) == 2.8f);
    
    REQUIRE(Vec3::distance({-2.8f, 4.2f, 10}, {8, 0.2f, 1.25f}) == 14.463834f);
}

TEST_CASE( "Vec3 lerp calculation", "[vec3]" ){
    REQUIRE(Vec3::lerp({0, 0, 0}, {1, 1, 1}, 0.248f) == Vec3{0.248f, 0.248f, 0.248f});
    REQUIRE(Vec3::lerp({1, 1, 1}, {0, 0, 0}, 0.248f) == Vec3{0.752f, 0.752f, 0.752f});
    REQUIRE(Vec3::lerp({0, 0, 0}, {-1, 2, 0}, 0.248f) == Vec3{-0.248f, 0.496f, 0.0f});
}

TEST_CASE( "Vec3 angle calculation", "[vec3]" ){
    REQUIRE(Vec3::angle({1, 0, 0}, {0, 1, 0}) == 90 * Mathf::DegToRad);
    REQUIRE(Vec3::angle({1, 0, 0}, {0, -45, 0}) == 90 * Mathf::DegToRad);
    
    REQUIRE(Vec3::angle({15, 1.24f, 0.4f}, {15, 1.24f, 0.4f}) == 0);
}

TEST_CASE( "Vec3 square magnitude calculation", "[vec3]" ){
    REQUIRE(Vec3(1, 0, 0).sqrMagnitude() == 1.0f);
    REQUIRE(Vec3(2, 2, 2).sqrMagnitude() == 12.0f);
    REQUIRE(Vec3(-2, 1, -2).sqrMagnitude() == 9.0f);
}

TEST_CASE( "Vec3 magnitude calculation", "[vec3]" ){
    REQUIRE(Vec3(1, 0, 0).magnitude() == 1.0f);
    REQUIRE(Vec3(2, 2, 2).magnitude() == 2 * Mathf::sqrt(3));
    REQUIRE(Vec3(-2, 1, -2).magnitude() == 3.0f);
}

TEST_CASE( "Vec3 normalization calculation", "[vec3]" ){
    REQUIRE(Vec3::normalize(Vec3(1, 0, 0)) == Vec3(1, 0, 0));
    REQUIRE(Vec3::normalize(Vec3(-2, 1, -2)) == Vec3(-2, 1, -2) / 3.0f);
    // REQUIRE(Vec3::normalize(Vec3()) == Vec3());
}

TEST_CASE( "Vec3 normalized calculation", "[vec3]" ){
    REQUIRE(Vec3(1, 0, 0).normalized() == Vec3(1, 0, 0));
    REQUIRE(Vec3(-2, 1, -2).normalized() == Vec3(-2, 1, -2) / 3.0f);
    // REQUIRE(Vec3::normalize(Vec3()) == Vec3());
}