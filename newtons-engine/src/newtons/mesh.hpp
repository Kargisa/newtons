#pragma once

#include "vec3.hpp"
#include "vec2.hpp"
#include <vector>
#include <cstdint>

namespace nwt{
struct Mesh{
    std::vector<Vec3> vertices;
    std::vector<uint32_t> indices;
    std::vector<Vec3> normals;
    std::vector<Vec2> texCoords;
    std::vector<Vec3> vertColors;

    Mesh(const std::vector<Vec3>& vertices, const std::vector<uint32_t>& indices, const std::vector<Vec2>& texCoords, const std::vector<Vec3>& vertColors)
        : vertices(vertices), indices(indices), texCoords(texCoords), vertColors(vertColors){}

    Mesh(const std::vector<Vec3>& vertices, const std::vector<uint32_t>& indices, const std::vector<Vec2>& texCoords)
        : vertices(vertices), indices(indices), texCoords(texCoords){}
    
    Mesh(const std::vector<Vec3>& vertices, const std::vector<uint32_t>& indices)
        : vertices(vertices), indices(indices){}

    void recalculateNormals();
};
}