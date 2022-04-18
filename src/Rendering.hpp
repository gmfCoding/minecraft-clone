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
    Mesh* mesh;
    const std::string& materialName;

    MeshRenderer(const std::string& material) : materialName(material)
    {

    }

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
    static std::map<std::string, Shader> shaders;

    static Camera* camera;

    static void CreateProgram(const std::string& name, const std::string& vertexPath, const std::string&  fragmentPath);
    static void DebugShaderInfo(GLuint shader);
    static void DebugProgramInfo(GLuint program);

    static void RenderObject(Object* object);
};