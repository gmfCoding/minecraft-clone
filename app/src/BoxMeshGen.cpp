#include "BoxMeshGen.hpp"
#include "Mesh.hpp"


BoxMeshGen::BoxMeshGen() : vertices(), triangles(), normals(), uvs()
{
    

}


Mesh BoxMeshGen::generate()
{
    // UPPER
    vertices.push_back(glm::vec3( 0.0,   0.5f,  0.5));
    vertices.push_back(glm::vec3( 0.5,   0.5f,  0.0));
    vertices.push_back(glm::vec3(-0.5,   0.5f,  0.0));
    vertices.push_back(glm::vec3( 0.0,   0.5f, -0.5));
    // LOWER    
    vertices.push_back(glm::vec3( 0.0,   -0.5f,  0.5));
    vertices.push_back(glm::vec3( 0.5,   -0.5f,  0.0));
    vertices.push_back(glm::vec3(-0.5,   -0.5f,  0.0));
    vertices.push_back(glm::vec3( 0.0,   -0.5f, -0.5));

#define TRIANGLE(i,j,k)     triangles.push_back(i); \
    triangles.push_back(j);                         \
    triangles.push_back(k);                         \

    // +Y
    TRIANGLE(0,1,4);
    TRIANGLE(1,2,3);
    // -Y
    TRIANGLE(4,5,7);
    TRIANGLE(5,6,7);
    // +X
    TRIANGLE(5,1,4);
    TRIANGLE(4,1,0);
    // -X
    TRIANGLE(7,2,6);
    TRIANGLE(6,7,2);
    // +Z
    TRIANGLE(6,2,5);
    TRIANGLE(5,2,1);
    // -Z
    TRIANGLE(7,5,3);
    TRIANGLE(3,5,0);


    uvs.push_back(glm::vec2(0,0));
    uvs.push_back(glm::vec2(0,0));
    uvs.push_back(glm::vec2(0,0));
    uvs.push_back(glm::vec2(0,0));

}
