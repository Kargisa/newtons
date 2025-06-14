# if (WIN32)
# add_executable (newtons-editor WIN32 "main.cpp" "obj_reader.hpp" "vertex.hpp" "vertex.cpp" "transformationMatrices.hpp" "mesh.cpp" "mesh.hpp" "stb_image.h")
# else()
add_executable(newtons-editor 

# Dear IMGUI
"imgui/imgui.cpp"
"imgui/imgui_demo.cpp"
"imgui/imgui_draw.cpp"
"imgui/imgui_tables.cpp"
"imgui/imgui_widgets.cpp"
"imgui/backends/imgui_impl_glfw.cpp"
"imgui/backends/imgui_impl_vulkan.cpp"
"imgui/misc/imgui_stdlib.cpp"

# Editor
"main.cpp"
)
# endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET newtons-engine PROPERTY CXX_STANDARD 26)
endif()

target_include_directories(newtons-editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_link_libraries(newtons-editor PRIVATE newtons-engine)