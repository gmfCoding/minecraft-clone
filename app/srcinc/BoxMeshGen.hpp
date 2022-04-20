#ifndef BOXMESHGEN_H
#define BOXMESHGEN_H
#include "Mesh.hpp"
#include <vector>
#include <glm.hpp>

class BoxMeshGen {

    public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned short> triangles;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    BoxMeshGen();
    Mesh generate();
};

#endif