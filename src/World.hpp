#pragma once
#include <glm/vec4.hpp>

class Renderer;
class Mesh;
class World;

struct Block
{
    glm::vec4 colour;

    Block() : colour(glm::vec4(0)) {}
};

class ChunkRenderer : public MeshRenderer {
    public:
    World* world;
    Mesh* mesh;
    int chunkX, chunkY, chunkZ;

    ChunkRenderer(World* _world) : MeshRenderer::MeshRenderer("world")
    {
        world = _world;
    }

    void Regenerate();
};

class World
{
    public:
    int sizeX;
    int sizeY;
    int sizeZ;

    int sizeXYZ;

    Block* map;

    glm::mat4 transform = glm::mat4(1.0);
    Mesh* mesh;
    ChunkRenderer* renderer;

    World(int _sizeX, int _sizeY, int _sizeZ);

    int GetIndex(int x, int y, int z);

    void Place(int x, int y, int z, glm::vec4 colour);

    void Render();

    static World* LoadWorld(const char* path);
};

