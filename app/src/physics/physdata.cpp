#include "physics/physdata.hpp"

AABB::AABB() : size{}, min{}, max{}
{

}

AABB::AABB(glm::vec3 size) : size(size), min(-(size* 0.5f)), max(size * 0.5f)
{

}

float AABB::dxSize()
{
    return max.x - min.x;
}

float AABB::dySize()
{
    return max.y - min.y;
}

float AABB::dzSize()
{
    return max.z - min.z;
}

glm::vec3 AABB::center()
{
    return glm::vec3(min.x + (dxSize()* 0.5f),
                        min.y + (dySize()* 0.5f), 
                        min.z + (dzSize()* 0.5f));
}

bool AABB::intersect(AABB target)
{
    return  min.x <= target.max.x &&
            max.x >= target.min.x &&
            min.y <= target.max.y &&
            max.y >= target.min.y &&
            min.z <= target.max.z &&
            max.z >= target.min.z;
}

bool AABB::pointInside(glm::vec3 point)
{
    return  point.x >= min.x &&
            point.x <= max.x &&
            point.y >= min.y &&
            point.y <= max.y &&
            point.z >= min.z &&
            point.z <= max.z;
}