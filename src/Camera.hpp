#pragma once
#include "Object.hpp"

class Camera : public Object
{   
public:
    glm::mat4 view;
    glm::mat4 projection;

    Camera(float fov, float aspect, float near, float far);

    void UpdateView();
};