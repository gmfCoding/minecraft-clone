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

#include "gldebug.hpp"
//#define GLCall(a) a


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
    renderer->m_mesh = new Mesh();
    renderer->transmesh = new Mesh();
}

int World::GetIndex(int x, int y, int z)
{
    return x * sizeY * sizeZ + y * sizeZ + z;
}

bool World::inBounds(glm::ivec3 pos)
{
    bool x = pos.x >= 0 && pos.x < this->sizeX;
    bool y = pos.y >= 0 && pos.y < this->sizeY;
    bool z = pos.z >= 0 && pos.z < this->sizeZ;
    return x & y & z;
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
    world->renderer->Bind(world->renderer->m_mesh);

    return world;
}


void ChunkRenderer::Regenerate()
{
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
void World::GenerateFace(glm::ivec3 dir, bool edge, glm::ivec3 pos, int32_t blockID)
{
    auto nIdx = GetIndex(pos.x + dir.x, pos.y + dir.y, pos.z + dir.z);   
    bool gen_condition = map[nIdx].blockID == 0;
    if(!gen_condition && (nIdx >= 0 && nIdx < sizeXYZ))
    {   
        Blocks::BlockConfig* ncfg = Blocks::GetConfig(map[nIdx].blockID);
        // Generate the face if:
        // It is air (pre setup)
        // It is trasparent
        // It is trasparent volume edge 
        gen_condition = (ncfg->flags & Blocks::BlockConfig::BlockFlags::Transparent) || ((ncfg->flags & Blocks::BlockConfig::BlockFlags::TransparentVol) && (blockID != map[nIdx].blockID));
    }

    if (edge || gen_condition)
    {
        Blocks::BlockConfig* config = Blocks::GetConfig(blockID);
        RectUV rectuvs = config->GetUVForFace(block::GLMVecToFACE(dir));

        auto vertices = &(renderer->m_mesh->vertices);
        auto indices = &(renderer->m_mesh->indices);
        auto uvs = &(renderer->m_mesh->uvs);
        bool transparent;
        if(config->flags & Blocks::BlockConfig::BlockFlags::Transparent || config->flags & Blocks::BlockConfig::BlockFlags::TransparentVol)
        {
            transparent = true;
            vertices = &(renderer->transmesh->vertices);
            indices = &(renderer->transmesh->indices);
            uvs = &(renderer->transmesh->uvs);
        }

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

        glm::vec3 offset = fpos + glm::vec3(0.5, 0.5, 0.5);
        vertices->push_back(offset + (-up + left + fdir) / 2.0f);
        vertices->push_back(offset + (up + left + fdir) / 2.0f );
        vertices->push_back(offset + (up - left + fdir) / 2.0f);
        vertices->push_back(offset + (-up - left + fdir) / 2.0f); 

        indices->push_back(o);                                   
        indices->push_back(o + 1);                               
        indices->push_back(o + 2);                               
        indices->push_back(o);                                   
        indices->push_back(o + 2);                               
        indices->push_back(o + 3); 

        uvs->push_back(rectuvs.topRight);
        uvs->push_back(rectuvs.topLeft);
        uvs->push_back(rectuvs.bottomRight);
        uvs->push_back(rectuvs.bottomLeft); 
        first = false;
    }
}

void ChunkRenderer::Bind(Mesh* mesh)
{
    BindOpaque();
    BindTransparent();
}

void ChunkRenderer::BindOpaque() {
    if(!vao_trans_gen) 
    {
        vao_gen = true;
        vbo_gen = true;
        ibo_gen = true;
        GLCall(glGenVertexArrays(1, &m_vao)); // Vertex  Array  Object
        GLCall(glGenBuffers(1, &m_vbo)); // Vertex  Buffer Object (temp)
        GLCall(glGenBuffers(1, &m_ibo)); // Vertex  Buffer Object (temp)
    }
    Bind(m_vao, m_vbo, m_ibo, &m_size, m_mesh);
}

void ChunkRenderer::BindTransparent() {
    if(!vao_trans_gen) 
    {
        vao_trans_gen = true;
        vbo_trans_gen = true;
        ibo_trans_gen = true;
        GLCall(glGenVertexArrays(1, &vao_trans)); // Vertex  Array  Object
        GLCall(glGenBuffers(1, &vbo_trans)); // Vertex  Buffer Object (temp)
        GLCall(glBindVertexArray(vao_trans));
        GLCall(glGenBuffers(1, &ibo_trans)); // Vertex  Buffer Object (temp)
    }

    Bind(vao_trans, vbo_trans, ibo_trans, &size_trans, transmesh);
}

void ChunkRenderer::Bind(GLuint vao, GLuint vbo, GLuint ibo, int* ibo_size, Mesh* mesh)
{
    GLCall(glBindVertexArray(vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    SetVertices(mesh, vao, vbo);
    SetIndices(mesh, vao, ibo, ibo_size, true);

    glBindVertexArray(0);
}


void ChunkRenderer::SetVerticesUV(Mesh* mesh, RectUV uv)
{
    GLCall(glBindVertexArray(m_vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glEnable( GL_BLEND );

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


    // GLCall(glDepthMask(GL_FALSE));
    // GLCall(glEnable(GL_BLEND));
    
    // GLCall(glBlendFunci(0, GL_ONE, GL_ONE));
    // GLCall(glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA));

    // Render opaque geometry
    GLCall(glBindVertexArray(m_vao));
    GLCall(glDrawElements(GL_TRIANGLES, m_size, GL_UNSIGNED_INT, nullptr));
    // GLboolean mode;

    // glGetBooleanv(GL_CULL_FACE, &mode);

    //glDisable(GL_CULL_FACE);
    // Render transparent geometry
    GLCall(glBindVertexArray(vao_trans)); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo_trans);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_trans);

    // Sort the triangles in transmesh
    transmesh->CameraSort(Renderer::camera->GetPosition());

    // Upload the sort triangle indices to the GPU
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned short) * transmesh->indices.size(), transmesh->indices.data());
    GLCall(glDrawElements(GL_TRIANGLES, size_trans, GL_UNSIGNED_SHORT, nullptr));

    // if(mode)
    //     glEnable(GL_CULL_FACE);

    // Unbind the VAO, VBO, and IBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}