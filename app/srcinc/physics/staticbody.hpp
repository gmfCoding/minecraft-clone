#include "transform.hpp"
#include "physdata.hpp"

class Staticbody
{
    public:

    Transform* transform;
    glm::vec3* position;
    AABB* aabb;

    Staticbody();

    virtual bool isStatic()
    {
        return true;
    }

    void RecalculateAABB();

};
