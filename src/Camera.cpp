#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"
#include "PlayerMove.h"

Camera::Camera(float fov, float aspect, float near, float far)
{
    this->projection = glm::perspective(
        glm::radians(fov), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        aspect,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        near,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        far             // Far clipping plane. Keep as little as possible.
    );
}

void Camera::UpdateView()
{
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // glm::vec3 cameraDirection = glm::normalize(this->position - cameraTarget);
    // glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    this->view = glm::lookAt(
        this->position,
        cameraTarget,
        up
    );
}


void Camera::Control(PlayerMove move)
{   
    glm::vec3* pos = PtrPosition();

    glm::vec3 cameraDirection = glm::normalize(*pos - glm::vec3(0.0f));
    glm::vec3 cameraLeft = -glm::normalize(glm::cross(glm::vec3(0,1,0), cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, -cameraLeft);

    if(move.Left) {
        *pos = *pos + cameraLeft;}
    if(move.Right) {*pos = *pos - cameraLeft;}
    if(move.Forward) {*pos = *pos + cameraDirection;}
    if(move.Backwards) {*pos = *pos - cameraDirection;}

    if(move.Up) {*pos = *pos + cameraUp;}
    if(move.Down) {*pos = *pos - cameraUp;}

    UpdateView();
}