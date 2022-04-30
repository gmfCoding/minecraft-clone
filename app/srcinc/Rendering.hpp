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
    GLuint vao, vbo, ibo;
    bool vao_gen, vbo_gen, ibo_gen;

    GLsizei size;
    GLuint texture;
    Mesh* mesh;
    const std::string& materialName;
    Shader* shader;

    MeshRenderer(const std::string& material) : materialName(material), vao_gen(false), vbo_gen(false), ibo_gen(false)
    {

    }

    void Render();
    void SetVertices(Mesh* mesh);
    void SetIndices(Mesh* mesh);

    
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