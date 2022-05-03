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

    Block() : blockID(0) {

    }
};

class ChunkRenderer : public MeshRenderer {
    World* world;



    public:
    int chunkX, chunkY, chunkZ;
    RectUV rectuv;
    Mesh* transmesh;

    GLuint vao_trans, vbo_trans, ibo_trans;
    bool vao_trans_gen, vbo_trans_gen, ibo_trans_gen;
    int size_trans;

    ChunkRenderer(World* _world) : MeshRenderer::MeshRenderer("world"), size_trans(0), vao_trans(0), vbo_trans(0), ibo_trans(0), vao_trans_gen(false), vbo_trans_gen(false), ibo_trans_gen(false)
    {
        world = _world;
    }

    void Regenerate();

    void Bind(Mesh* mesh);

    void BindOpaque();
    void BindTransparent();

    void Bind(GLuint vao, GLuint vbo, GLuint ibo, int* ibo_size, Mesh* mesh);

    void SetVerticesUV(Mesh* mesh, RectUV uv);
    
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

    ChunkRenderer* renderer;

    World(int _sizeX, int _sizeY, int _sizeZ);

    int GetIndex(int x, int y, int z);

    void Place(int x, int y, int z, int32_t colour);

    static World* LoadWorld(const char* path);

    void GenerateFace(glm::ivec3 dir, bool cond, glm::ivec3 offset, int32_t blockID);
};

