cd $(dirname "$0")


glslc shader.vert -o ./compiledShaders/vert.spv
glslc shader.frag -o ./compiledShaders/frag.spv

glslc triangle.vert -o ./compiledShaders/triangleVert.spv
glslc triangle.frag -o ./compiledShaders/triangleFrag.spv