#include <glm/vec3.hpp>
#include "Rendering.hpp"
#include "World.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <vector>

#include "Rendering.hpp"
#include "TextureManager.hpp"
#include "MaterialSystem.hpp"
#include "Material.hpp"
#include "CommonData.hpp"
#include "Blocks.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

//#include "gldebug.hpp"
#define GLCall(a) a


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

void World::Place(int x, int y, int z, int32_t blockID)
{
    map[GetIndex(x, y, z)].blockID = blockID;
}


class Object;

World *World::LoadWorld(const char *path)
{
    std::ifstream i(getAssetPath(path));
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

    for (size_t i = 0; i < voxels.size(); i++)
    {
        int64_t col = j["palette"][voxels[i][3]];
        world->Place(voxels[i][0], voxels[i][1], voxels[i][2], col);
    }

    world->renderer->Regenerate();
    world->renderer->Bind(world->mesh);

    return world;
}


void ChunkRenderer::Regenerate()
{

    world->mesh = mesh;
    auto vertices = &(mesh->vertices);
    auto indices = &(mesh->indices);
    auto uvs = &(mesh->uvs);
    auto tmpIdx = 0;

#define BLOCK(x, y, z) world->map[world->GetIndex(x, y, z)]

    for (size_t x = 0; x < world->sizeX; x++)
    {
        for (size_t y = 0; y < world->sizeY; y++)
        {
            for (size_t z = 0; z < world->sizeZ; z++)
            {
                Block b = BLOCK(x, y, z);
                // If the block is "air" continue/skip.
                if (b.blockID == 0)
                {
                    continue;
                }

                glm::ivec3 t = glm::vec3(x, y, z);
                glm::ivec3 a = glm::ivec3(0,1,0);
                glm::ivec3 u = glm::ivec3(0,-1,0);
                glm::ivec3 n = glm::ivec3(0,0,1);
                glm::ivec3 s = glm::ivec3(0,0,-1);
                glm::ivec3 e = glm::ivec3(1,0,0);
                glm::ivec3 w = glm::ivec3(-1,0,0);


                // Generate Faces for each block, with conditions if those faces should be created.
                world->GenerateFace(a, t.y + a.y >= world->sizeY, t, b.blockID);
                world->GenerateFace(e, t.x + e.x >= world->sizeX, t, b.blockID);
                world->GenerateFace(n, t.z + n.z >= world->sizeZ, t, b.blockID);

                world->GenerateFace(s, t.z + s.z == -1, t, b.blockID);
                world->GenerateFace(w, t.x + w.x == -1, t, b.blockID);
                world->GenerateFace(u, t.y + u.y == -1, t, b.blockID);
            }
        }
    }

    #undef BLOCK 
}

bool first = true;
void World::GenerateFace(glm::ivec3 dir, bool cond, glm::ivec3 pos, int32_t blockID)
{
    auto tmpIdx = GetIndex(pos.x + dir.x, pos.y + dir.y, pos.z + dir.z);   
    if (cond || (tmpIdx >= 0 && tmpIdx < sizeXYZ) && map[GetIndex(pos.x + dir.x, pos.y + dir.y, pos.z + dir.z)].blockID == 0)
    {
        auto vertices = &(mesh->vertices);
        auto indices = &(mesh->indices);
        auto uvs = &(mesh->uvs);

        glm::vec3 fdir = glm::vec3(dir);
        glm::vec3 fpos = glm::vec3(pos);

        glm::vec3 zero = glm::vec3(0);
        glm::vec3 w_up = glm::vec3(0,1,0);
        glm::vec3 left = -glm::cross(glm::vec3(w_up), glm::vec3(dir));


        if(fdir == w_up || fdir == -w_up)
        {
            left = glm::cross(glm::vec3(1,0,0), fdir);
        }

        glm::vec3 up = -glm::cross(left, fdir);


        int o = vertices->size();


        vertices->push_back(fpos + (-up + left + fdir) / 2.0f);
        vertices->push_back(fpos + (up + left + fdir) / 2.0f );
        vertices->push_back(fpos + (up - left + fdir) / 2.0f);
        vertices->push_back(fpos + (-up - left + fdir) / 2.0f); 

        indices->push_back(o);                                   
        indices->push_back(o + 1);                               
        indices->push_back(o + 2);                               
        indices->push_back(o);                                   
        indices->push_back(o + 2);                               
        indices->push_back(o + 3);      

        RectUV rectuvs = Blocks::GetUVForFace(block::GLMVecToFACE(dir), blockID);

        uvs->push_back(rectuvs.topRight);
        uvs->push_back(rectuvs.topLeft);
        uvs->push_back(rectuvs.bottomRight);
        uvs->push_back(rectuvs.bottomLeft); 
        first = false;
    }
}

void ChunkRenderer::Bind(Mesh* mesh)
{
#pragma region VAOVBOIBO
    glBindVertexArray(0);

    if(!vao_gen) 
    {
        vao_gen = true;
        vbo_gen = true;
        GLCall(glGenVertexArrays(1, &vao)); // Vertex  Array  Object
        GLCall(glGenBuffers(1, &vbo)); // Vertex  Buffer Object (temp)
    }

    GLCall(glBindVertexArray(vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    SetVertices(mesh);

    if(!ibo_gen)
    {
        ibo_gen = true;
        GLCall(glGenBuffers(1, &ibo)); // Element Buffer Object (temp)
    }

    SetIndices(mesh);

    glBindVertexArray(0);

#pragma endregion

#pragma region TEXTURES

    auto [textureID, img] = TextureManager::LoadTextureGPU("resources/textures/grass_top.png");
    texture = textureID;

    // bind the texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set the filter to nearest
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width = img->width;
    int height = img->height;

    // set texture content
    // TODO Try without the following
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->c_data);
    glGenerateMipmap(GL_TEXTURE_2D);
#pragma endregion
}



void ChunkRenderer::SetVerticesUV(Mesh* mesh, RectUV uv)
{
    GLCall(glBindVertexArray(vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    
    // Contains UV's
    if(mesh->uvs.size() >= 1 && mesh->uvs.size() == mesh->vertices.size())
    {
        std::vector<Vertex> vertices_uv;
        for (size_t i = 0; i < mesh->vertices.size(); i++)
        {
            vertices_uv.push_back({mesh->vertices[i], ((glm::vec2*)&uv)[i % 4]});
        }
        GLCall(glBufferData(GL_ARRAY_BUFFER, vertices_uv.size() * sizeof(Vertex), vertices_uv.data(), GL_STATIC_DRAW)); 
        // Location of position
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
        // Location of Texcoords
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, texcoord))));
    }
    // No UV's
    else
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(glm::vec3), mesh->vertices.data(), GL_STATIC_DRAW));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0));
    }
}


void ChunkRenderer::Render()
{
    Material* mat = MaterialSystem::materialMap["alt_textured_debug"];
    auto program = mat->programID;

    GLCall(glUseProgram(program));

    GLCall(GLuint uniTexture = glGetUniformLocation(program, "tex"));
    GLCall(GLuint uniTransform = glGetUniformLocation(program, "MVP"));
    
    if(mat->defaults.HasValue("dsp"))
    {
        bool active = MaterialSystem::materialMap["alt_textured_debug"]->defaults.GetValue<bool>("dsp");
        GLCall(GLuint dspOn = glGetUniformLocation(program, "dspUV"));
        glUniform1i(dspOn, active ? 1 : 0);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureManager::GetNamedTexture("worldatlas"));
    
    glUniform1i(uniTexture, 0);


    glm::mat4 mvp = Renderer::camera->projection * Renderer::camera->view * world->transform;
    GLCall(glUniformMatrix4fv(uniTransform, 1, GL_FALSE,  glm::value_ptr(mvp)));

    GLCall(glBindVertexArray(vao));
    GLCall(glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr));

    glBindVertexArray(0);
}