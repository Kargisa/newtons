#pragma once

#include "vec3.hpp"
#include "vec2.hpp"
#include <vector>
#include <cstdint>

namespace nwt {
    class Mesh {
        std::vector<Vec3> _vertices;
        std::vector<Vec3> _normals;
        std::vector<Vec3> _vertexColors;
        std::vector<Vec2> _texCoords;
        std::vector<uint32_t> _indices;

    public:
        Mesh(const std::vector<Vec3>& vertices, const std::vector<uint32_t>& indices, const std::vector<Vec2>& texCoords, const std::vector<Vec3>& vertColors)
            : _vertices(vertices), _indices(indices), _texCoords(texCoords), _vertexColors(vertColors) {
        }

        Mesh(const std::vector<Vec3>& vertices, const std::vector<uint32_t>& indices, const std::vector<Vec2>& texCoords)
            : _vertices(vertices), _indices(indices), _texCoords(texCoords) {
        }

        Mesh(const std::vector<Vec3>& vertices, const std::vector<uint32_t>& indices)
            : _vertices(vertices), _indices(indices) {
        }

        std::vector<Vec3>& vertices();
        std::vector<Vec3>& normals();
        std::vector<Vec3>& vertexColors();
        std::vector<Vec2>& texCoords();
        std::vector<uint32_t>& indices();

    };
}