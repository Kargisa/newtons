#include <catch2/catch_test_macros.hpp>
#include "vec2.hpp"

using namespace nwt;


TEST_CASE( "Vec2 direction checks", "[vec2]" ) {
    REQUIRE(Vec2::up() == Vec2{0, 1});
    REQUIRE(Vec2::down() == Vec2{0, -1});
    
    REQUIRE(Vec2::right() == Vec2{1, 0});
    REQUIRE(Vec2::left() == Vec2{-1, 0});
}

TEST_CASE( "Vec2 sclae calculation", "[vec2]" ){
    REQUIRE(Vec2::scale({0, 0}, {2.4515f, 1.0f}) == Vec2{0, 0});
    REQUIRE(Vec2::scale({2.4515f, 1.0f}, {0, 0}) == Vec2{0, 0});

    REQUIRE(Vec2::scale({3, 1}, {8, 16.458f}) == Vec2{24, 16.458f});
    REQUIRE(Vec2::scale({8, 16.458f}, {3, 1}) == Vec2{24, 16.458f});

}

TEST_CASE( "Vec2 dot product calculation", "[vec2]" ){
    REQUIRE(Vec2::dot({0, 0}, {2.4515f, 1.0f}) == 0);
    REQUIRE(Vec2::dot({2.4515f, 1.0f}, {0, 0}) == 0);

    REQUIRE(Vec2::dot({2, 1}, {2, 1}) == 5);
    REQUIRE(Vec2::dot({1.54f, 8}, {7, 13.87f}) == 121.74f);
}

TEST_CASE( "Vec2 distance calculation", "[vec2]" ){
    REQUIRE(Vec2::distance({0, 0}, {2.8f, 0}) == 2.8f);
    REQUIRE(Vec2::distance({-2.8f, 0}, {0, 0}) == 2.8f);
    
    REQUIRE(Vec2::distance({-2.8f, 4.2f}, {8, 0.2f}) == 11.516944039f);
}

TEST_CASE( "Vec2 lerp calculation", "[vec2]" ){
    REQUIRE(Vec2::lerp({0, 0}, {1, 1}, 0.248f) == Vec2{0.248f, 0.248f});
    REQUIRE(Vec2::lerp({1, 1}, {0, 0}, 0.248f) == Vec2{0.752f, 0.752f});
    REQUIRE(Vec2::lerp({0, 0}, {-1, 2}, 0.248f) == Vec2{-0.248f, 0.496f});
}

TEST_CASE( "Vec2 angle calculation", "[vec2]" ){
    REQUIRE(Vec2::angle({1, 0}, {0, 1}) == 90 * Mathf::DegToRad);
    REQUIRE(Vec2::angle({1, 0}, {0, -45}) == 90 * Mathf::DegToRad);
    
    REQUIRE(Vec2::angle({15, 1.24f}, {15, 1.24f}) == 0);
}

TEST_CASE( "Vec2 square magnitude calculation", "[vec2]" ){
    REQUIRE(Vec2(1, 0).sqrMagnitude() == 1.0f);
    REQUIRE(Vec2(2, 2).sqrMagnitude() == 8.0f);
    REQUIRE(Vec2(-2, 1).sqrMagnitude() == 5.0f);
}

TEST_CASE( "Vec2 magnitude calculation", "[vec2]" ){
    REQUIRE(Vec2(1, 0).magnitude() == 1.0f);
    REQUIRE(Vec2(2, 2).magnitude() == Mathf::sqrt(8.0f));
    REQUIRE(Vec2(-2, 1).magnitude() == Mathf::sqrt(5.0f));
}

TEST_CASE( "Vec2 normalization calculation", "[vec2]" ){
    REQUIRE(Vec2::normalize(Vec2(1, 0)) == Vec2(1, 0));
    REQUIRE(Vec2::normalize(Vec2(-2, 1)) == Vec2(-2, 1) / Mathf::sqrt(5.0f));
    // REQUIRE(Vec3::normalize(Vec3()) == Vec3());
}

TEST_CASE( "Vec2 normalized calculation", "[vec2]" ){
    REQUIRE(Vec2(1, 0).normalized() == Vec2(1, 0));
    REQUIRE(Vec2(-2, 1).normalized() == Vec2(-2, 1) / Mathf::sqrt(5.0f));
    // REQUIRE(Vec3::normalize(Vec3()) == Vec3());
}