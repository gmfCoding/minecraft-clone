#include <glm/vec3.hpp>

class AABB
{
    glm::vec3 min;
    glm::vec3 max;
    
    float xSize();
    float ySize();
    float zSize();

    glm::vec3 center();

    bool intersect(AABB target);

    bool pointInside(glm::vec3 point);
};


float AABB::xSize()
{
    return max.x - min.x;
}

float AABB::ySize()
{
    return max.y - min.y;
}

float AABB::zSize()
{
    return max.z - min.z;
}

glm::vec3 AABB::center()
{
    return glm::vec3(min.x + (xSize()/2.0f),
                        min.y + (ySize()/2.0f), 
                        min.z + (zSize()/2.0f));
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