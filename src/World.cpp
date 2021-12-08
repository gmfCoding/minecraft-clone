#include <glm/vec3.hpp>
#include "Rendering.hpp"
#include "World.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <vector>

inline int fastrand(int *sc_seed) { 
  *sc_seed = (214013 * (*sc_seed) + 2531011); 
  return (*sc_seed>>16)&0x7FFF; 
}

World::World(int _sizeX, int _sizeY, int _sizeZ, Object* object) : sizeX(_sizeX), sizeY(_sizeY), sizeZ(_sizeZ), sizeXYZ(sizeX * sizeY * sizeZ), map(nullptr)
{
    map = new Block[sizeX * sizeY * sizeZ];
    for (size_t i = 0; i < sizeXYZ; i++)
    {
        map[i] = Block();
    }

    defaultCube = new Object();
    defaultCube->renderer = object->renderer;
}

int World::GetIndex(int x, int y, int z)
{
    return x * sizeY * sizeZ + y * sizeZ + z;
}

void World::Place(int x, int y, int z, glm::vec4 colour)
{
    map[GetIndex(x,y,z)].colour = colour;
}

void World::Render()
{
    int seed = 0;
    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            for (int k = 0; k < sizeZ; k++) {
                Block block = map[GetIndex(i,j,k)];
                if(block.colour.w <= 0.0f) {continue;}

                defaultCube->colour = block.colour * (90.1f + (fastrand(&seed) % 11)) / 100.0f;;

                defaultCube->SetPosition(glm::vec3(i,j,k));
                Renderer::RenderObject(defaultCube);
            }
        }
    }
}

struct VoxelData {
    int x;
    int y;
    int z;
    int pallet;
};

class Object;

World* World::LoadWorld(const char* path, Object *object)
{
    std::ifstream i(path);
    nlohmann::json j;
    i >> j;
    std::vector<std::vector<int64_t>> voxels = j["layers"][0]["voxels"].get<std::vector<std::vector<int64_t>>>();
    
    int maxSize[3] {0,0,0};


    for (size_t i = 0; i < voxels.size(); i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            if(maxSize[j] < voxels[i][j]+1) { maxSize[j] = voxels[i][j]+1; }
        }
    }

    World* world = new World(maxSize[0], maxSize[1], maxSize[2], object);
    
#define ConvertHEXtoRGBA(colour) glm::vec4(((colour >> 16) & 0xFF) / 255.0,((colour >> 8) & 0xFF) / 255.0, ((colour) & 0xFF) / 255.0 ,1.0f )
    for (size_t i = 0; i < voxels.size(); i++)
    {
        int64_t col = j["palette"][voxels[i][3]];
        world->Place(voxels[i][0],voxels[i][1],voxels[i][2],ConvertHEXtoRGBA(col));
    }

    return world;
}