#pragma once
#include <glm/gtc/quaternion.hpp>
#include <string>
#include "Rendering.hpp"

class Mesh {
    public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned short> indices;
};

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

enum ShaderType {FRAGMENT = 0x8B30, VERTEX = 0x8B31};
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
    static void CreateProgram(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    static void DebugShaderInfo(GLuint shader);
    static void DebugProgramInfo(GLuint program);
    static void RenderObject(Object* object);
};

class Object {
public:
    char* name;
    Mesh* mesh;
    enum Flags {
        DirtyMat = 0b00000001,
        Example1 = 0b00000010,
        Example2 = 0b00000100,
        Example3 = 0b00001000,
        Example4 = 0b00010000,
        Example5 = 0b00100000,
        Example6 = 0b01000000,
        Example7 = 0b10000000 };
    unsigned char flags;
    MeshRenderer* renderer;
    Shader* shader;
public:
    glm::vec3* PtrPosition();
    glm::vec3* PtrScale();
    glm::quat* PtrRotation();
    glm::mat4* PtrTransform();
    glm::vec3 GetPosition();
    glm::vec3 GetScale();
    glm::quat GetRotation();
    glm::mat4 GetTransform();
    void SetPosition(glm::vec3 pos);
    void SetScale(glm::vec3 pos);
    void SetRotation(glm::quat pos);
    void SetMatrix(glm::mat4 mat);
    void UpdateTransform();
    Object();
    glm::mat4 transform;
private:
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
};

class Camera : public Object
{
public:
    glm::mat4 view;
    glm::mat4 projection;
};
