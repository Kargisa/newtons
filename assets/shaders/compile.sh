cd $(dirname "$0")

glslc shader.vert -o vert.spv
glslc shader.frag -o frag.spv

cp vert.spv compiledShaders
cp frag.spv compiledShaders

glslc shader.vert -o triangleVert.spv
glslc shader.frag -o triangleFrag.spv


cp triangleVert.spv compiledShaders
cp triangleFrag.spv compiledShaders