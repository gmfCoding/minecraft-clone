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

    GLCall(glBindVertexArray(renderer->vao));
    GLCall(glDrawElements(GL_TRIANGLES, renderer->size, GL_UNSIGNED_INT, nullptr));

    // GLuint uniTransform = glGetUniformLocation(program, "model");
    // glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(object->GetTransform()));

    // GLuint uniTransform = glGetUniformLocation(program, "view");
    // glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(camera.GetView());

    // GLuint uniTransform = glGetUniformLocation(program, "projection");
    // glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(camera.GetProjection());



}

void MeshRenderer::Bind(Mesh* mesh)    {

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
    
    size = mesh->indices.size();

    GLuint vbo, ibo;
    GLCall(glGenVertexArrays(1, &vao)); // Vertex  Array  Object
    GLCall(glGenBuffers(1, &vbo)); // Vertex  Buffer Object (temp)
    GLCall(glGenBuffers(1, &ibo)); // Element Buffer Object (temp)

    GLCall(glBindVertexArray(vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    if(hasUVs)
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));    
    }
    else 
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(glm::vec3), mesh->vertices.data(), GL_STATIC_DRAW));
    }
        
    GLCall(glEnableVertexAttribArray(0));


    if(hasUVs)
    {
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
    }
    else 
    {
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0));
    }

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Index), indices.data(), GL_STATIC_DRAW));

    if(hasUVs)
    {
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, texcoord))));
    }

    GLCall(glBindVertexArray(0));
}
    
void MeshRenderer::Render()
{

}