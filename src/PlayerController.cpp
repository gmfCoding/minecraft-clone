#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "PlayerController.hpp"
#include "Input.hpp"

void PlayerController::Control(PlayerMove move)
{
    float moveSpeed = 0.2f;
    glm::vec3* pos = camera->PtrPosition();

    glm::vec3 cameraLeft = glm::cross(glm::vec3(0,1,0), camera->direction);
    glm::vec3 cameraUp =  -glm::normalize(glm::cross(camera->direction, -cameraLeft));

    if(move.Left) {
        *pos = *pos + cameraLeft * moveSpeed;}
    if(move.Right) {*pos = *pos - cameraLeft * moveSpeed;}

    if(move.Forward) {*pos = *pos + camera->direction * moveSpeed;}
    if(move.Backwards) {*pos = *pos - camera->direction * moveSpeed;}

    if(move.Up) {*pos = *pos + cameraUp * moveSpeed;}
    if(move.Down) {*pos = *pos - cameraUp * moveSpeed;}
}

void PlayerController::OnMouseInput(void* _input)
{
    Input* input = reinterpret_cast<Input*> (_input);
    float sensitivity = 0.3f;

    camera->yaw   += input->mouseXDelta * sensitivity;
    camera->pitch += input->mouseYDelta * sensitivity;

    
    if( camera->pitch > 89.0f)
         camera->pitch =  89.0f;
    if( camera->pitch < -89.0f)
         camera->pitch = -89.0f;

    camera->direction.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->direction.y = sin(glm::radians(camera->pitch));
    camera->direction.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->direction = glm::normalize(camera->direction);
}