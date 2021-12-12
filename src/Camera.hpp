#pragma once
#include "Object.hpp"
#include "PlayerMove.h"

class Camera : public Object
{   
public:
    glm::mat4 view;
    glm::mat4 projection;

    Camera(float fov, float aspect, float near, float far);

    void UpdateView();

    void Control(PlayerMove move);
};