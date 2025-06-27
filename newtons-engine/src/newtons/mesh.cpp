#include "mesh.hpp"

namespace nwt {
    std::vector<Vec3>& Mesh::vertices() {
        return _vertices;
    }

    std::vector<Vec3>& Mesh::normals() {
        return _normals;
    }

    std::vector<Vec3>& Mesh::vertexColors() {
        return _vertexColors;
    }

    std::vector<Vec2>& Mesh::texCoords() {
        return _texCoords;
    }

    std::vector<uint32_t>& Mesh::indices() {
        return _indices;
    }
}