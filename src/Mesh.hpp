#pragma once

#include <vector>
#include <glm/glm.hpp>

class Mesh {
    public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned short> indices;
};