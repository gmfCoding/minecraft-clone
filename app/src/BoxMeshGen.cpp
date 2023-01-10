#include "BoxMeshGen.hpp"
#include "Mesh.hpp"
#include <vector>


BoxMeshGen::BoxMeshGen() : vertices(), triangles(), normals(), uvs()
{
    

}


Mesh BoxMeshGen::generate()
{
    Mesh mesh;
    auto &vertices = mesh.vertices;
    auto &triangles = mesh.indices;
    auto &uvs = mesh.uvs;
 
    std::vector<float> cube_vertices {
        // front
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        // back
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f
    };

    for (size_t i = 0; i < cube_vertices.size(); i += 3)
    {
        vertices.push_back(glm::vec3(cube_vertices[i], cube_vertices[i + 1], cube_vertices[i + 2]));
    }
    
    	/* init_resources */
	triangles = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};


    uvs.push_back(glm::vec2(0,0));
    uvs.push_back(glm::vec2(0,0));
    uvs.push_back(glm::vec2(0,0));
    uvs.push_back(glm::vec2(0,0));
    return mesh;
}
