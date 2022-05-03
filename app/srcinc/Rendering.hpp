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
    GLuint m_vao, m_vbo, m_ibo;
    bool vao_gen, vbo_gen, ibo_gen;

    GLuint texture;
    Mesh* m_mesh;
    GLsizei m_size;
    const std::string& materialName;
    Shader* shader;

    MeshRenderer(const std::string& material) : materialName(material), m_size(0), texture(0), vao_gen(false), vbo_gen(false), ibo_gen(false)
    {

    }

    void Render();
    void SetVertices(Mesh* mesh, int vao, int vbo);
    void SetIndices(Mesh* mesh, int vao, int ibo, int* size);

    
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