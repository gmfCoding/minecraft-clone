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
#include "CommonData.hpp"
#include "Blocks.hpp"

#include <glm/gtc/type_ptr.hpp>


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

#define ConvertHEXtoRGBA(colour) glm::vec4(((colour >> 16) & 0xFF) / 255.0, ((colour >> 8) & 0xFF) / 255.0, ((colour)&0xFF) / 255.0, 1.0f)
    for (size_t i = 0; i < voxels.size(); i++)
    {
        int64_t col = j["palette"][voxels[i][3]];
        world->Place(voxels[i][0], voxels[i][1], voxels[i][2], col);
    }

    world->renderer->Regenerate();


    //glGenerateMipmap(GL_TEXTURE_2D);
    world->renderer->Bind(world->renderer->mesh);

    return world;
}




void ChunkRenderer::Regenerate()
{
    auto vertices = &(mesh->vertices);
    auto indices = &(mesh->indices);
    auto uvs = &(mesh->uvs);
    auto tmpIdx = 0;

#define ConvertRGBAtoHEX(vec) ((int(vec.x * 255) & 0xff) << 24) + ((int(vec.y * 255) & 0xff) << 16) + ((int(vec.z * 255) & 0xff) << 8) + (int(vec.w * 255) & 0xff);

#define BLOCK(x, y, z) world->map[world->GetIndex(x, y, z)]
#define INRANGE(ind) ind >= 0 && ind < world->sizeXYZ
#define NVALID(b) world->map[world->GetIndex(b.x, b.y, b.z)].blockID == 0

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
    RectUV rectuvs = Blocks::GetUVForFace(block::GLMVecToFACE(d - t), b.blockID);\
    uvs->push_back(rectuvs.bottomRight);\
    uvs->push_back(rectuvs.topRight);\
    uvs->push_back(rectuvs.topLeft);\
    uvs->push_back(rectuvs.bottomLeft);\
}


    // uvs->push_back(glm::vec2(0.0, 0.0));                          \
    // uvs->push_back(glm::vec2(0.0, 1.0));                          \
    // uvs->push_back(glm::vec2(1.0, 1.0));                          \
    // uvs->push_back(glm::vec2(1.0, 0.0));                          \

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

                //(((b.colour.x * 255) & 0xff) << 24) + (((b.colour.y * 255.0f) & 0xff) << 16) + (((b.colour.z * 255.0f) & 0xff) << 8) + ((b.colour.w * 255.0f) & 0xff);

                glm::vec3 t = glm::vec3(x, y, z);
                glm::vec3 a = glm::vec3(x, y + 1, z);
                glm::vec3 u = glm::vec3(x, y - 1.0f, z);
                glm::vec3 n = glm::vec3(x, y, z + 1);
                glm::vec3 s = glm::vec3(x, y, z - 1.0f);
                glm::vec3 e = glm::vec3(x + 1, y, z);
                glm::vec3 w = glm::vec3(x - 1.0f, y, z);

                // Generate Faces for each block, with conditions if those faces should be created.
                FACE(a, a.y >= world -> sizeY, x, y + 1, z,     x + 1, y + 1, z,    x + 1, y + 1, z + 1,    x, y + 1, z + 1)
                FACE(n, n.z >= world -> sizeZ, x, y, z + 1,     x, y + 1, z + 1,    x + 1, y + 1, z + 1,    x + 1, y, z + 1)

                //FACE(e, e.x >= world -> sizeX, x + 1, y, z,     x + 1, y, z + 1,    x + 1, y + 1, z + 1,    x + 1, y + 1, z);
                FACE(e, e.x >= world -> sizeX, x + 1, y, z+1,     x + 1, y+1, z + 1,    x + 1, y + 1, z,    x + 1, y, z);
                FACE(s, s.z == -1,             x, y, z,     x, y+1, z,    x + 1, y + 1, z,   x + 1, y, z)

                FACE(w, w.x == -1,             x, y, z,     x, y + 1, z,    x, y + 1, z + 1,   x, y, z + 1);
                FACE(u, u.y == -1,             x, y, z,     x, y, z + 1,    x + 1, y, z + 1,   x + 1, y, z);
            }
        }
    }
}


void ChunkRenderer::Bind(Mesh* mesh)
{
#pragma region VertexOraganisation
    std::vector<Vertex> vertices;
    std::vector<Index> indices;

    bool hasUVs = false;
    if(mesh->uvs.size() >= 1 && mesh->uvs.size() == mesh->vertices.size())
    {
        hasUVs = true;
        for (size_t i = 0; i < mesh->vertices.size(); i++)
        {
            vertices.push_back({mesh->vertices[i], mesh->uvs[i]});
        }
    }
    else
    {
        for(auto v : mesh->vertices)
        {
            vertices.push_back({v, {0,0}});
        }
    }

    for(auto i : mesh->indices)
    {
        indices.push_back(i);
    }

    size = indices.size();
#pragma endregion

#pragma region VAOVBOIBO
    glBindVertexArray(0);

    GLuint vbo, ibo;
    GLCall(glGenVertexArrays(1, &vao)); // Vertex  Array  Object -- Links multiple VBOs and auto binds them
    GLCall(glGenBuffers(1, &vbo)); // Vertex  Buffer Object -- Is an array in memory that may or may not contain multple data types such as --
    // vertices, vertex colours, uv's etc, use a vao with glVertexAttribPointer to describe where and the layout of each of these data types.
    // it could be layed out as such vertex1_pos, vertex1_uv, vertex2_pos, vertex2_uv,vertex3_pos, vertex3_uv

    GLCall(glBindVertexArray(vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    // fill the vbo, with GLfloat * vertices.size() amount of floats, the data being at &vertices[0], static drawing
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size() * 5, vertices.data(), GL_STATIC_DRAW);
                    
    // VERTICES
    // bind the current vao to the current vbo with the following vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));
    // shader access index, 3 (xyz),  of GL_FLOAT,       dont transpose,   array buffer stride (vertices, uv,... repeat)   ,offset until first vertices component inside of array buffer (0 in our case)
 
    // UVs
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT,   GL_FALSE,         5*sizeof(GLfloat),                              (char*)0 + 3*sizeof(GLfloat));
    // shader access index, 2 (u & v), of GL_FLOAT,       dont transpose,   array buffer stride (vertices, uv,... repeat)   ,offset until UVs inside of array buffer
    
    
    // Generate and bind the index buffer object so we can fill it with data
    glGenBuffers(1, &ibo);
 
    // Fill the currently bound GL_ELEMENT_ARRAY_BUFFER buffer (ibo) with the data in indices
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Index), indices.data(), GL_STATIC_DRAW));
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
    
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

void ChunkRenderer::Render()
{
    auto program = MaterialSystem::materialMap["alt_textured"]->programID;

    GLCall(glUseProgram(program));

    GLCall(GLuint uniTexture = glGetUniformLocation(program, "tex"));
    GLCall(GLuint uniTransform = glGetUniformLocation(program, "MVP"));
    

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureManager::GetNamedTexture("worldatlas"));
    
    glUniform1i(uniTexture, 0);


    glm::mat4 mvp = Renderer::camera->projection * Renderer::camera->view * world->transform;
    GLCall(glUniformMatrix4fv(uniTransform, 1, GL_FALSE,  glm::value_ptr(mvp)));

    GLCall(glBindVertexArray(vao));
    GLCall(glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr));

    glBindVertexArray(0);
}