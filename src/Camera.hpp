#pragma once
#include "Object.hpp"
#include "PlayerMove.hpp"

class Input;

class Camera : public Object
{   
public:
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 direction;

    float yaw = 225.0f;
    float pitch = 0.0f;

    Camera(float fov, float aspect, float near, float far);

    void UpdateView();
};