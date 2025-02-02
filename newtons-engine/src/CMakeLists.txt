# if (WIN32)
# add_executable (newtons-editor WIN32 "main.cpp" "obj_reader.hpp" "vertex.hpp" "vertex.cpp" "transformationMatrices.hpp" "mesh.cpp" "mesh.hpp" "stb_image.h")
# else()
add_library(newtons-engine STATIC "newtons/main.cpp" "newtons/mesh.cpp" "newtons/application.cpp" "newtons/window/window.cpp" )
# endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET newtons-engine PROPERTY CXX_STANDARD 26)
endif()

find_package(Vulkan REQUIRED)

target_link_libraries(newtons-engine PRIVATE newtons-utils PRIVATE glfw PRIVATE Vulkan::Vulkan)

target_include_directories(newtons-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(newtons-engine PRIVATE "newtons/pch.hpp")