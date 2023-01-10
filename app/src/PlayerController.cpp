#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>

#include "engine.hpp"
#include "Camera.hpp"
#include "PlayerController.hpp"
#include "Input.hpp"

#include <iostream>

#include "glad/glad.h"
#include "Rendering.hpp"
#include "World.hpp"

#include "Mineclone.hpp"
#include "Blocks.hpp"
#include "imgui/imgui.h"

void PlayerController::Control(PlayerMove move)
{
    ProcessMovement();
    
    float moveSpeed = 6.6f * currentEngine->deltaTime;
    glm::vec3* pos = camera->PtrPosition();

    glm::vec3 cameraLeft = glm::normalize(glm::cross(glm::vec3(0,1,0), camera->direction));
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
}

void PlayerController::ProcessMovement()
{  
    World* world = Mineclone::instance->world;
    glm::vec3 wPos = *eyeposition;
    // Invert the worldTransform matrix
    
    glm::mat4 inverseWorldTransform = glm::inverse(world->transform);
    glm::vec3 pos = inverseWorldTransform * glm::vec4(*eyeposition - glm::vec3(0, height, 0), 1.0);

    if(pos.x < 0)
        pos.x = floor(pos.x);
    if(pos.y < 0)
        pos.y = floor(pos.y);
    if(pos.z < 0)
        pos.z = floor(pos.z);

    glm::ivec3 below = glm::ivec3(pos.x, pos.y, pos.z);




    int nIdx = world->GetIndex(below.x, below.y, below.z);
    bool shouldFall = false;

    ImGui::InputFloat3("FUCKOIFFSDF", (float*)&pos);


    if(nIdx >= 0 && nIdx < world->sizeXYZ && world->inBounds(below))
    {
        auto id = world->map[nIdx].blockID;
        Blocks::BlockConfig* config = Blocks::GetConfig(id);
        if(id < 1 || config->flags & Blocks::BlockConfig::BlockFlags::Transparent)
        {
            shouldFall  = true;
            
        }else if(id >= 1)
        {
            shouldFall = false;
            ImGui::Text(config->name.c_str());
            eyeposition->y = (world->transform * glm::vec4(0,below.y + .999f + height,0, 1)).y;
        }
    }else{
        shouldFall = true;
    }

    ImGui::InputInt3("BlockPosBelow:", (int*)&below);
    ImGui::Text("SHOULD FALL: %i", shouldFall);
    
    if(shouldFall)
        *eyeposition -= glm::vec3(0,1 * currentEngine->deltaTime, 0);
}