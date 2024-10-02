# CMakeList.txt : CMake project for newtons-cpp, include source and define
# project specific logic here.
#

# Add source to this project's executable.

add_executable (newtons-editor "main.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET newtons-editor PROPERTY CXX_STANDARD 20)
endif()

find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(newtons-editor PRIVATE newtons-engine PRIVATE glfw PRIVATE Vulkan::Vulkan PRIVATE glm::glm)

add_subdirectory("shaders")

# TODO: Add tests and install targets if needed.
