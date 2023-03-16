#include "physics/transform.hpp"
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <unordered_map>

Transform::Transform() : transform(glm::mat4(1.0f)), position({0,0,0}), scale({1,1,1}), rotation(glm::quat())
{

}

void Transform::SetPosition(glm::vec3 pos) { TRANS_SETDIRTY; position = pos;}
void Transform::SetScale(glm::vec3 sca)  { TRANS_SETDIRTY; scale = sca;}
void Transform::SetRotation(glm::quat rot) { TRANS_SETDIRTY; rotation = rot;}
void Transform::SetMatrix(glm::mat4 mat) { TRANS_SETDIRTY; transform = mat;}

glm::vec3 Transform::GetPosition() const { return position; }
glm::vec3 Transform::GetScale() const { return scale; }
glm::quat Transform::GetRotation() const { return rotation; }
glm::mat4 Transform::GetTransform() const
{ 
    if(flags & Transform::Flags::Update)
        UpdateTransform();
    return transform;     
}

glm::vec3* Transform::PtrPosition() { TRANS_SETDIRTY; return &position; }
glm::vec3* Transform::PtrScale() { TRANS_SETDIRTY; return &scale; }
glm::quat* Transform::PtrRotation() { TRANS_SETDIRTY; return &rotation; }
glm::mat4* Transform::PtrTransform() { return &transform; }


void Transform::UpdateTransform() const
{
    TRANS_CLEAR_DIRTY; 
    transform = glm::translate(glm::mat4(1.0f), position);
    transform = transform * glm::mat4_cast(rotation); 
    transform = glm::scale(transform, scale);
    return;
    transform = glm::translate(glm::mat4(1.0f), position) * (glm::mat4(1.0f) * glm::mat4_cast(rotation)) * glm::scale(glm::mat4(1.0f), scale);
}


// void Transform::ImGui_TransformTransform()
// {
//     static std::unordered_map<void*, glm::vec3> eulerMap = std::unordered_map<void*, glm::vec3>(); 
//     auto rot = PtrRotation();
//     if(!eulerMap.count(rot))
//         eulerMap.emplace((void*)rot, glm::degrees(glm::eulerAngles(*rot)));
        
//     glm::vec3 &euler = eulerMap[rot];
//     //euler = glm::eulerAngles(*rot);

//     ImGui::DragFloat3("position:", &(PtrPosition()->x), 0.1f);
//     ImGui::DragFloat3("rotation:", &(euler.x), 0.1f);
//     ImGui::DragFloat3("scale:", &(PtrScale()->x), 0.1f);
    
//     glm::quat QuatAroundX = glm::angleAxis(glm::radians(euler.x),  glm::vec3(1.0,0.0,0.0));
//     glm::quat QuatAroundY = glm::angleAxis(glm::radians(euler.y), glm::vec3(0.0,1.0,0.0));
//     glm::quat QuatAroundZ = glm::angleAxis(glm::radians(euler.z), glm::vec3(0.0,0.0,1.0));
    
//     glm::quat finalOrientation = QuatAroundX * QuatAroundY * QuatAroundZ;
//     SetRotation(finalOrientation);
//     //eulerMap[rot] = euler;
// }
