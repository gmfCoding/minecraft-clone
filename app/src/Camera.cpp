#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.hpp"
#include "PlayerMove.hpp"
#include "Input.hpp"

Camera::Camera(float fov, float aspect, float near, float far)
{
    this->projection = glm::perspective(
        glm::radians(fov), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        aspect,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        near,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        far             // Far clipping plane. Keep as little as possible.
    );
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(direction);
}

void Camera::UpdateView()
{
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(direction);

    this->view = glm::lookAt(
        this->position,
        this->position + direction,
        up
    );
}