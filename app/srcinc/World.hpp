#pragma once
#include <glm/vec4.hpp>
#include <map>
#include "CommonData.hpp"

class Renderer;
class Mesh;
class World;

struct Block
{
    public:
    int blockID;

    Block() {

    }
};

class ChunkRenderer : public MeshRenderer {
    World* world;
    public:
    Mesh* mesh;
    int chunkX, chunkY, chunkZ;

    ChunkRenderer(World* _world) : MeshRenderer::MeshRenderer("world")
    {
        world = _world;
    }

    void Regenerate();


    void Bind(Mesh* mesh);
    
    void Render();
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

    void Place(int x, int y, int z, int32_t colour);

    static World* LoadWorld(const char* path);
};

