
#include <unordered_map>
#include <vector>
#include <cmath>

#include "physics/physdata.hpp"
#include "physics/staticbody.hpp"
#include "physics/rigidbody.hpp"
#include "physics/physutil.hpp"

class CollisionSystem
{
    public:
    std::unordered_map<Staticbody*, AABB*> staticToBoxes;
    std::unordered_map<AABB*, Staticbody*> aabbToStaticbodies;

    std::vector<AABB*> alignedBoxes;

    CollisionSystem();

    void Add(Staticbody* body);

    public:
    void ApplyCollisions();

    void Collide(AABB* first, AABB* second);

    glm::vec3 AABBCollisionNormal(AABB abox, AABB bbox, glm::vec3);
};


