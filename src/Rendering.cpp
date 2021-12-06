#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "gldebug.hpp"
#include "fileio.hpp"

#include "Rendering.hpp"
#include "Object.hpp"
#include "Camera.hpp"

std::map<std::string, Shader> Renderer::shaders = std::map<std::string, Shader>();
Camera* Renderer::camera;

void Renderer::CreateProgram(const std::string& name, const std::string& vertexPath, const std::string&  fragmentPath)
{
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertShaderStr = readFile(vertexPath);
    std::string fragShaderStr = readFile(fragmentPath);
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);
    DebugShaderInfo(vertShader);

    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);
    DebugShaderInfo(fragShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    DebugProgramInfo(program);

    Shader shader = Shader();
    shader.shaderProgram = program;
    Renderer::shaders[name] = shader;


    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

void Renderer::DebugShaderInfo(GLuint shader)
{
    GLint result = GL_FALSE;
    int logLength;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> shaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(shader, logLength, NULL, &shaderError[0]);
    std::cout << &shaderError[0] << std::endl;
}

void Renderer::DebugProgramInfo(GLuint program)
{
    GLint result = GL_FALSE;
    int logLength;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError( (logLength > 1) ? logLength : 1 );
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;
}

void Renderer::RenderObject(Object* object)
{    
    MeshRenderer* renderer = object->renderer;
    auto program = shaders["default"].shaderProgram;
    
    glm::mat4 transform = glm::mat4(1.0f);

    // GLfloat* tFloatPtr = glm::value_ptr(transform);
    glm::mat4* tPtr = object->PtrTransform();
    glm::mat4 mvp = camera->projection * camera->view * object->GetTransform();

    GLCall(glUseProgram(program));
    
    GLCall(GLuint uniColour = glGetUniformLocation(program, "col_uni"));
    GLCall(glUniform4fv(uniColour, 1, &(object->colour).x));//&(tPtr[0][0]).x)

    GLCall(GLuint uniTransform = glGetUniformLocation(program, "transform"));
    GLCall(glUniformMatrix4fv(uniTransform, 1, GL_FALSE,  glm::value_ptr(mvp)));//&(tPtr[0][0]).x)

    // GLuint uniTransform = glGetUniformLocation(program, "model");
    // glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(object->GetTransform()));

    // GLuint uniTransform = glGetUniformLocation(program, "view");
    // glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(camera.GetView());

    // GLuint uniTransform = glGetUniformLocation(program, "projection");
    // glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(camera.GetProjection());


    GLCall(glBindVertexArray(renderer->vao));
    GLCall(glDrawElements(GL_TRIANGLES, renderer->size, GL_UNSIGNED_INT, nullptr));
}

using Index = unsigned int;

struct Vertex {
    glm::vec3  position;
    glm::vec2  texcoord;
};


void MeshRenderer::Bind(Mesh* mesh)    {

        std::vector<Vertex> vertices;
        std::vector<Index> indices;
        for(auto v : mesh->vertices)
        {
            vertices.push_back({v, {0,0}});
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
        // Works for the Vertex struct (glm::vec3 pos, glm::vec2 texcoords)
        // GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        // GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));

        // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        // GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Index), indices.data(), GL_STATIC_DRAW));

        // GLCall(glEnableVertexAttribArray(0));
        // GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));

        GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        GLCall(glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(glm::vec3), mesh->vertices.data(), GL_STATIC_DRAW));

        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Index), indices.data(), GL_STATIC_DRAW));

        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0));

        // GLCall(glEnableVertexAttribArray(1));
        // GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, texcoord))));

        GLCall(glBindVertexArray(0));
    }
    
//      {
//     size = mesh->indices.size();
//     glGenVertexArrays(1, &vao);
//     glGenBuffers(1, &vbo);
//     glGenBuffers(1, &ebo);
//     glBindVertexArray(vao);

//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(mesh->vertices) * sizeof(glm::vec3), &mesh->vertices[0], GL_STATIC_DRAW);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
//     glEnableVertexAttribArray(0);

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned short), &mesh->indices[0], GL_STATIC_DRAW);

//     glBindBuffer(GL_ARRAY_BUFFER, 0); 
// }