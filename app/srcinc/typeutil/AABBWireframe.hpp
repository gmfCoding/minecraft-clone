#include <vector>
#include <glm/vec3.hpp>
#include "physics/physdata.hpp"

namespace AABBWireframe
{
    std::vector<glm::vec3> CreateWriteframe(AABB* box)
    {
        glm::vec3 pos = box->center();
        glm::vec3 min = box->min;
        glm::vec3 max = box->max;
        float dx = box->dxSize() * 0.5f;
        float dy = box->dySize() * 0.5f;
        float dz = box->dzSize() * 0.5f;

        glm::vec3 llb = {pos.x - dx, pos.y - dy, pos.z - dz};
        glm::vec3 lrb = {pos.x + dx, pos.y - dy, pos.z - dz};
        glm::vec3 llf = {pos.x - dx, pos.y - dy, pos.z + dz};
        glm::vec3 lrf = {pos.x + dx, pos.y - dy, pos.z + dz};
        glm::vec3 ulb = {pos.x - dx, pos.y + dy, pos.z - dz};
        glm::vec3 urb = {pos.x + dx, pos.y + dy, pos.z - dz};
        glm::vec3 ulf = {pos.x - dx, pos.y + dy, pos.z + dz};
        glm::vec3 urf = {pos.x + dx, pos.y + dy, pos.z + dz};


        return {
            llb, lrb, lrb, lrf, lrf, llf, llf,llb, // LOWER SQUARE
            llb, ulb, lrb, urb, lrf, urf, llf, ulf, // SIDE LINES
            ulb, urb, urb, urf, urf, ulf, ulf,ulb // UPPER SQUARE
        };
    }
}