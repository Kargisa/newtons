#include <catch2/catch_test_macros.hpp>
#include "quaternion.hpp"

using namespace nwt;

TEST_CASE( "Quaternion identity check", "[quaternion]" ){
    REQUIRE(Quaternion::identity() == Quaternion(1, 0, 0, 0));
}

TEST_CASE( "Quaternion conjugate calculation", "[quaternion]" ){
    REQUIRE(Quaternion::conjugate({0.1f, 0.5f, 0.4f, 1}) == Quaternion(0.1f, -0.5f, -0.4f, -1));
    REQUIRE(Quaternion::conjugate({0, 0, 0, 0}) == Quaternion(0, 0, 0, 0));
    REQUIRE(Quaternion::conjugate({-1, 2, -3.1f, 0}) == Quaternion(-1, -2, 3.1f, 0));
}

TEST_CASE( "Quaternion conjugated calculation", "[quaternion]" ){
    REQUIRE(Quaternion(0.1f, 0.5f, 0.4f, 1).conjugated() == Quaternion(0.1f, -0.5f, -0.4f, -1));
    REQUIRE(Quaternion(0, 0, 0, 0).conjugated() == Quaternion(0, 0, 0, 0));
    REQUIRE(Quaternion(-1, 2, -3.1f, 0).conjugated() == Quaternion(-1, -2, 3.1f, 0));
}

TEST_CASE( "Quaternion rotate vector calculation", "[quaternion]" ){
    REQUIRE(Quaternion::rotateVector({1, 0, 0, 0}, {12, 1.15f, 0}) == Vec3(12, 1.15f, 0));
    REQUIRE(Quaternion::rotateVector({Mathf::cos(45 * Mathf::DegToRad), Mathf::sin(45 * Mathf::DegToRad) * Vec3(0, 1, 0)}, {12, 1.15f, 0}) == Vec3(0, 1.15f, -12));
    REQUIRE(Quaternion::rotateVector({Mathf::cos(45 * Mathf::DegToRad), Mathf::sin(45 * Mathf::DegToRad) * Vec3(1, 0, 0)}, {12, 1.15f, 0}) == Vec3(12, 0, 1.15f));
    REQUIRE(Quaternion::rotateVector({Mathf::cos(45 * Mathf::DegToRad), Mathf::sin(45 * Mathf::DegToRad) * Vec3(0, 0, 1)}, {12, 1.15f, 0}) == Vec3(-1.15f, 12, 0));
}

TEST_CASE( "Quaternion form euler calculation", "[quaternion]" ){
    REQUIRE(Quaternion::fromEuler(0, 0, 0) == Quaternion(1, 0, 0, 0));
    REQUIRE(Quaternion::fromEuler(45 * Mathf::DegToRad, 20  * Mathf::DegToRad, 30  * Mathf::DegToRad) == Quaternion(0.8960407f, 0.3225058f, 0.2525045f, 0.1712969f));
}