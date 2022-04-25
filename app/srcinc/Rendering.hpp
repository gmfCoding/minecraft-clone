#pragma once
#include <map>
#include <string>
#include "LoadGlad.h"
#include "fileio.hpp"

#include "Mesh.hpp"
#include "Camera.hpp"

class Object;

struct MeshRenderer
{
    GLuint vao;
    GLsizei size;
    GLuint texture;
    Mesh* mesh;
    const std::string& materialName;
    Shader* shader;

    MeshRenderer(const std::string& material) : materialName(material)
    {

    }

    void Render();

    void Bind(Mesh* mesh);
};

enum ShaderType {FRAGMENT = GL_FRAGMENT_SHADER, VERTEX = GL_VERTEX_SHADER};

struct CreateProgramArgument {
    std::string& path;
    ShaderType type;
};

class Shader {
    public:
    GLuint shaderProgram;
};

class Renderer {
    public:
    static Camera* camera;
    
    static void RenderObject(const Object* object);
};

using Index = unsigned int;

struct Vertex {
    glm::vec3  position;
    glm::vec2  texcoord;
};