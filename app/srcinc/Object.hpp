#pragma once
#include <glm/gtc/quaternion.hpp>
#include <string>

struct MeshRenderer;
class Shader;
class Mesh;


#define OBJ_DIRTYMATRIX 0x0
#define OBJ_SETDIRTY flags |= Object::Flags::DirtyMat;
#define OBJ_CLEAR_DIRTY flags &= ~Object::Flags::DirtyMat;

class Object {
public:
    char* name;
    Mesh* mesh;

    glm::vec4 colour;

    enum Flags {
        DirtyMat    = 0b00000001,
        Example1    = 0b00000010,
        Example2    = 0b00000100,
        Example3    = 0b00001000,
        Example4    = 0b00010000,
        Example5    = 0b00100000,
        Example6    = 0b01000000,
        Example7    = 0b10000000 };

    mutable unsigned char flags;

    MeshRenderer* renderer;

public:
    glm::vec3* PtrPosition();
    glm::vec3* PtrScale();
    glm::quat* PtrRotation();
    glm::mat4* PtrTransform();

    glm::vec3 GetPosition() const;
    glm::vec3 GetScale() const;
    glm::quat GetRotation() const;
    glm::mat4 GetTransform() const;

    void SetPosition(glm::vec3 pos);
    void SetScale(glm::vec3 pos);
    void SetRotation(glm::quat pos);
    void SetMatrix(glm::mat4 mat);

    void UpdateTransform() const;


    Object();
    mutable glm::mat4 transform;
    
protected:
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
};