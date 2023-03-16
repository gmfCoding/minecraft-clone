#pragma once
#include <glm/vec3.hpp>
class AABB
{
    public:
    glm::vec3 min;
    glm::vec3 max;

    glm::vec3 size;
    
    AABB();
    AABB(glm::vec3 size);

    float dxSize();
    float dySize();
    float dzSize();

    glm::vec3 center();

    bool intersect(AABB target);

    bool pointInside(glm::vec3 point);
};
