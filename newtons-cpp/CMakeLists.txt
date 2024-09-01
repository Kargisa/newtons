# CMakeList.txt : CMake project for newtons-cpp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (newtons-cpp "newtons-cpp.cpp"  "Vector3.h" "Math.h" "Vector2.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET newtons-cpp PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
