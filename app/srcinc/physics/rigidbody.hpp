#include <glm/vec3.hpp>
#include "physics/physdata.hpp"
#include "physics/transform.hpp"

class Rigidbody : public Staticbody
{
    public:
    glm::vec3 velocity;
    
    bool isStatic()
    {
        return false;
    }
};


