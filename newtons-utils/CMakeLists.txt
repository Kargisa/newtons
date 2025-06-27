# CMakeList.txt : CMake project for newtons-cpp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (newtons-utils INTERFACE)


if (CMAKE_VERSION VERSION_GREATER 3.12)
set_property(TARGET newtons-utils PROPERTY CXX_STANDARD 23)
endif()

target_include_directories(newtons-utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set(NWT_UTILS_TESTS OFF CACHE BOOL "Enable Testing for NEWTONS Utils")

if (NWT_UTILS_TESTS)
add_subdirectory("tests")
endif()