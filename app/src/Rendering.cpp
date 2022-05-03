#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "LoadGlad.h"


#include "gldebug.hpp"
#include "fileio.hpp"
#include "AtulPwd.hpp"

#include "Rendering.hpp"
#include "Object.hpp"
#include "Camera.hpp"

#include "MaterialSystem.hpp"

Camera* Renderer::camera;

void Renderer::RenderObject(const Object* object)
{    
    MeshRenderer* renderer = object->renderer;
    auto program = MaterialSystem::materialMap["default"]->programID;

    GLCall(glUseProgram(program));
    
    GLCall(GLuint uniColour = glGetUniformLocation(program, "col_uni"));
    GLCall(glUniform4fv(uniColour, 1, &(object->colour).x));

    glm::mat4 mvp = camera->projection * camera->view * object->GetTransform();

    GLCall(GLuint uniTransform = glGetUniformLocation(program, "transform"));
    GLCall(glUniformMatrix4fv(uniTransform, 1, GL_FALSE,  glm::value_ptr(mvp)))

    GLCall(glBindVertexArray(renderer->m_vao));
    GLCall(glDrawElements(GL_TRIANGLES, renderer->m_size, GL_UNSIGNED_INT, nullptr));
}

void MeshRenderer::Bind(Mesh* mesh)    {
    bool hasUVs = mesh->uvs.size() >= 1 && mesh->uvs.size() == mesh->vertices.size();

    if(vao_gen == false) 
    {
        vao_gen = true;
        GLCall(glGenVertexArrays(1, &m_vao)); // Vertex  Array  Object
        GLCall(glGenBuffers(1, &m_vbo)); // Vertex  Buffer Object (temp)
        GLCall(glGenBuffers(1, &m_ibo)); // Element Buffer Object (temp)
    }

    GLCall(glBindVertexArray(m_vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

    SetVertices(mesh, m_vao, m_vbo);
    SetIndices(mesh, m_vao, m_ibo, &m_size);


    GLCall(glBindVertexArray(0));
}

void MeshRenderer::SetVertices(Mesh* mesh, int vao = -1, int vbo = -1)
{
    if(vao == -1)
        vao = this->m_vao;
    if(vbo == -1)
        vbo = this->m_vbo;
        
    GLCall(glBindVertexArray(vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    
    // Contains UV's
    if(mesh->uvs.size() >= 1 && mesh->uvs.size() == mesh->vertices.size())
    {
        std::vector<Vertex> vertices_uv;
        for (size_t i = 0; i < mesh->vertices.size(); i++)
        {
            vertices_uv.push_back({mesh->vertices[i], mesh->uvs[i]});
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


void MeshRenderer::SetIndices(Mesh* mesh, int vao = -1, int ibo = -1, int* size = nullptr)
{
    if(vao == -1)
        vao = this->m_vao;
    if(ibo == -1)
        ibo = this->m_ibo;
    if(size == nullptr)
        size = &this->m_size;

    GLCall(glBindVertexArray(vao));

    std::vector<Index> indices;

    for(auto i : mesh->indices)
    {
        indices.push_back(i);
    }

    *size = indices.size();

    // NOTE: Do we need a currently bound vao to buffer/bind the ibo?
    // Fill the currently bound GL_ELEMENT_ARRAY_BUFFER buffer (ibo) with the data in indices
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Index), indices.data(), GL_STATIC_DRAW));
}

void MeshRenderer::Render()
{

}