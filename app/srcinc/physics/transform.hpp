#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/quaternion_float.hpp>

#define TRANS_DIRTYMATRIX 0x0
#define TRANS_SETDIRTY flags |= Transform::Flags::Update;
#define TRANS_CLEAR_DIRTY flags &= ~Transform::Flags::Update;


class Transform {
        enum Flags {
        Update      = 0b00000001,
        Example1    = 0b00000010,
        Example2    = 0b00000100,
        Example3    = 0b00001000,
        Example4    = 0b00010000,
        Example5    = 0b00100000,
        Example6    = 0b01000000,
        Example7    = 0b10000000 };

    mutable unsigned char flags;

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

    // void ImGui_ObjectTransform();

    Transform();
    mutable glm::mat4 transform;
    
public:
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
};