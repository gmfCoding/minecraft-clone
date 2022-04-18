#include <glm/vec3.hpp>
#include "Rendering.hpp"
#include "World.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

inline int fastrand(int *sc_seed)
{
    *sc_seed = (214013 * (*sc_seed) + 2531011);
    return (*sc_seed >> 16) & 0x7FFF;
}

World::World(int _sizeX, int _sizeY, int _sizeZ) : sizeX(_sizeX), sizeY(_sizeY), sizeZ(_sizeZ), sizeXYZ(sizeX * sizeY * sizeZ), map(nullptr)
{
    map = new Block[sizeX * sizeY * sizeZ];
    for (size_t i = 0; i < sizeXYZ; i++)
    {
        map[i] = Block();
    }

    renderer = new ChunkRenderer(this);
    renderer->mesh = new Mesh();
}

int World::GetIndex(int x, int y, int z)
{
    return x * sizeY * sizeZ + y * sizeZ + z;
}

void World::Place(int x, int y, int z, glm::vec4 colour)
{
    map[GetIndex(x, y, z)].colour = colour;
}

void World::Render()
{
    int seed = 0;

    auto program = Renderer::shaders["default"].shaderProgram;

    // GLfloat* tFloatPtr = glm::value_ptr(transform);
    Renderer::camera->UpdateView();
    glm::mat4 mvp = Renderer::camera->projection * Renderer::camera->view * transform;
#define GLCall(a) a
    GLCall(glUseProgram(program));
    auto col = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    GLCall(GLuint uniColour = glGetUniformLocation(program, "col_uni"));
    GLCall(glUniform4fv(uniColour, 1, &(col).x));

    GLCall(GLuint uniTransform = glGetUniformLocation(program, "transform"));
    GLCall(glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(mvp))); //&(tPtr[0][0]).x)

    // GLuint uniTransform = glGetUniformLocation(program, "model");
    // glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(object->GetTransform()));

    // GLuint uniTransform = glGetUniformLocation(program, "view");
    // glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(camera.GetView());

    // GLuint uniTransform = glGetUniformLocation(program, "projection");
    // glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(camera.GetProjection());

    GLCall(glBindVertexArray(renderer->vao));
    GLCall(glDrawElements(GL_TRIANGLES, renderer->size, GL_UNSIGNED_INT, nullptr));
}

struct VoxelData
{
    int x;
    int y;
    int z;
    int pallet;
};


struct VoxelVertexData {
    glm::vec3 position;
    glm::vec2 uv;
};

class Object;

World *World::LoadWorld(const char *path)
{
    std::ifstream i(getFilePath(path));
    nlohmann::json j;
    i >> j;
    std::vector<std::vector<int64_t>> voxels = j["layers"][0]["voxels"].get<std::vector<std::vector<int64_t>>>();

    int maxSize[3]{0, 0, 0};
    // Compute the max size the world would need to be to fit the loading world.
    for (size_t i = 0; i < voxels.size(); i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            if (maxSize[j] < voxels[i][j] + 1)
            {
                maxSize[j] = voxels[i][j] + 1;
            }
        }
    }

    World *world = new World(maxSize[0], maxSize[1], maxSize[2]);

#define ConvertHEXtoRGBA(colour) glm::vec4(((colour >> 16) & 0xFF) / 255.0, ((colour >> 8) & 0xFF) / 255.0, ((colour)&0xFF) / 255.0, 1.0f)
    for (size_t i = 0; i < voxels.size(); i++)
    {
        int64_t col = j["palette"][voxels[i][3]];
        world->Place(voxels[i][0], voxels[i][1], voxels[i][2], ConvertHEXtoRGBA(col));
    }

    world->renderer->Regenerate();
    world->renderer->Bind(world->renderer->mesh);

    return world;
}

#define FACE(d, cond, x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4) \
    tmpIdx = world->GetIndex(d.x, d.y, d.z);                          \
    if (cond || (INRANGE(tmpIdx) && NVALID(d)))                       \
    {                                                                 \
        int o = vertices->size();                                     \
        vertices->push_back(glm::vec3(x1, y1, z1));                   \
        vertices->push_back(glm::vec3(x2, y2, z2));                   \
        vertices->push_back(glm::vec3(x3, y3, z3));                   \
        vertices->push_back(glm::vec3(x4, y4, z4));                   \
        indices->push_back(o);                                        \
        indices->push_back(o + 2);                                    \
        indices->push_back(o + 1);                                    \
        indices->push_back(o);                                        \
        indices->push_back(o + 3);                                    \
        indices->push_back(o + 2);                                    \
    }


void ChunkRenderer::Regenerate()
{
    auto vertices = &(mesh->vertices);
    auto indices = &(mesh->indices);
    auto tmpIdx = 0;

#define BLOCK(x, y, z) world->map[world->GetIndex(x, y, z)]
#define INRANGE(ind) ind >= 0 && ind < world->sizeXYZ
#define NVALID(b) world->map[world->GetIndex(b.x, b.y, b.z)].colour.w <= 0.0f

    for (size_t x = 0; x < world->sizeX; x++)
    {
        for (size_t y = 0; y < world->sizeY; y++)
        {
            for (size_t z = 0; z < world->sizeZ; z++)
            {
                Block b = BLOCK(x, y, z);
                // If the block is "air" continue.
                if (b.colour.w <= 0.0f)
                {
                    continue;
                }

                glm::vec3 a = glm::vec3(x, y + 1, z);
                glm::vec3 u = glm::vec3(x, y - 1.0f, z);
                glm::vec3 n = glm::vec3(x, y, z + 1);
                glm::vec3 s = glm::vec3(x, y, z - 1.0f);
                glm::vec3 e = glm::vec3(x + 1, y, z);
                glm::vec3 w = glm::vec3(x - 1.0f, y, z);

                // Generate Faces for each block, with conditions if those faces should be created.
                FACE(a, a.y >= world->sizeY, x, y + 1, z, x + 1, y + 1, z, x + 1, y + 1, z + 1, x, y + 1, z + 1)
                FACE(n, n.z >= world->sizeZ, x, y, z + 1, x, y + 1, z + 1, x + 1, y + 1, z + 1, x + 1, y, z + 1)
                FACE(e, e.x >= world->sizeX, x+1,y,z, x+1,y,z+1, x+1,y+1,z+1,x+1,y+1,z);
                FACE(s, s.z == -1, x, y, z, x + 1, y, z, x + 1, y + 1, z,  x, y + 1, z)
                FACE(w, w.x == -1, x,y,z, x,y+1,z, x,y+1,z+1, x,y,z+1);
                FACE(u, u.y == -1, x,y,z, x,y,z+1, x+1,y,z+1, x+1,y,z);
            }
        }
    }
}