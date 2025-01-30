# CMakeList.txt : CMake project for newtons-cpp, include source and define
# project specific logic here.
#

# Add source to this project's executable.

# if (WIN32)
# add_executable (newtons-editor WIN32 "main.cpp" "obj_reader.hpp" "vertex.hpp" "vertex.cpp" "transformationMatrices.hpp" "mesh.cpp" "mesh.hpp" "stb_image.h")
# else()
add_executable (newtons-editor "main.cpp" "obj_reader.hpp" "vertex.hpp" "vertex.cpp" "transformationMatrices.hpp" "mesh.cpp" "mesh.hpp" "stb_image.h" "transform.hpp")
# endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET newtons-editor PROPERTY CXX_STANDARD 26)
endif()

find_package(Vulkan REQUIRED)

target_link_libraries(newtons-editor PRIVATE newtons-utils PRIVATE glfw PRIVATE Vulkan::Vulkan)


file (COPY "shaders/compiledShaders/" DESTINATION shaders)
file (COPY "imgs" DESTINATION .)
file (COPY "models" DESTINATION .)

# TODO: Add tests and install targets if needed.