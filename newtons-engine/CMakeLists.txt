# CMakeList.txt : CMake project for newtons-cpp, include source and define
# project specific logic here.
#

# Add source to this project's executable.



file (COPY "shaders/compiledShaders/" DESTINATION shaders)
file (COPY "imgs" DESTINATION .)
file (COPY "models" DESTINATION .)

add_subdirectory(src)

# TODO: Add tests and install targets if needed.