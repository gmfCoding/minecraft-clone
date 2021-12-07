#pragma once

#include <glm/vec4.hpp>

class Object;


struct Block
{
    glm::vec4 colour;

    Block() : colour(glm::vec4(0)) {}
};


class World
{
    public:
    int sizeX;
    int sizeY;
    int sizeZ;

    int sizeXYZ;

    Block* map;

    Object* defaultCube;

    World(int _sizeX, int _sizeY, int _sizeZ, Object* object);

    int GetIndex(int x, int y, int z);

    void Place(int x, int y, int z, glm::vec4 colour);

    void Render();

    static World* LoadWorld(const char* path, Object *object);
};