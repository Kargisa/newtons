Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

# These tests can use the Catch2-provided main
add_executable(newtons-utils-test "vec2_test.cpp" "vec3_test.cpp" "vec4_test.cpp" "mat4x4_test.cpp" "quaternion_test.cpp")

# target_link_libraries(newtons-utils-test PRIVATE newtons-utils)
target_link_libraries(newtons-utils-test PRIVATE newtons-utils PRIVATE Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(newtons-utils-test)
# add_test(newtons-utils-test-t newtons-utils-test)