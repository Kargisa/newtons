# CMakeList.txt : CMake project for newtons-cpp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (newtons-utils INTERFACE "vec3.hpp" "mathf.hpp" "vec2.hpp" "mat4x4.hpp" "vec4.hpp" "hash.hpp" "quaternion.hpp")


if (CMAKE_VERSION VERSION_GREATER 3.12)
set_property(TARGET newtons-utils PROPERTY CXX_STANDARD 26)
endif()

target_include_directories(newtons-utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# option(ENABLE_TESTS "Enable unit tests" OFF)

# if (ENABLE_TESTS)
add_subdirectory("tests")
# endif()