#include "mesh.hpp"

namespace nwt{
void Mesh::recalculateNormals()
{
    normals.clear();

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        Vec3 v1 = vertices[indices[i + 0]];
        Vec3 v2 = vertices[indices[i + 1]];
        Vec3 v3 = vertices[indices[i + 2]];

        Vec3 v12 = v2 - v1;
        Vec3 v13 = v3 - v1;
    }
}
}