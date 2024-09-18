# CMakeList.txt : CMake project for newtons-cpp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (newtons-engine STATIC "main.cpp" "Vector3.h" "Mathf.h" "Vector2.h" )


if (CMAKE_VERSION VERSION_GREATER 3.12)
set_property(TARGET newtons-engine PROPERTY CXX_STANDARD 20)
endif()



target_include_directories(newtons-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})