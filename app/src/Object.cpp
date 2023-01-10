#include "Object.hpp"
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <unordered_map>

Object::Object() : transform(glm::mat4(1.0f)), position({0,0,0}), scale({1,1,1}), rotation(glm::quat()), renderer(nullptr)
{

}

void Object::SetPosition(glm::vec3 pos) { OBJ_SETDIRTY; position = pos;}
void Object::SetScale(glm::vec3 sca)  { OBJ_SETDIRTY; scale = sca;}
void Object::SetRotation(glm::quat rot) { OBJ_SETDIRTY; rotation = rot;}
void Object::SetMatrix(glm::mat4 mat) { OBJ_CLEAR_DIRTY; transform = mat;}

glm::vec3 Object::GetPosition() const { return position; }
glm::vec3 Object::GetScale() const { return scale; }
glm::quat Object::GetRotation() const { return rotation; }
glm::mat4 Object::GetTransform() const
{ 
    if(flags & Object::Flags::DirtyMat)
        UpdateTransform();
    return transform;     
}

glm::vec3* Object::PtrPosition() { OBJ_SETDIRTY; return &position; }
glm::vec3* Object::PtrScale() { OBJ_SETDIRTY; return &scale; }
glm::quat* Object::PtrRotation() { OBJ_SETDIRTY; return &rotation; }
glm::mat4* Object::PtrTransform() { return &transform; }


void Object::UpdateTransform() const
{
    OBJ_CLEAR_DIRTY; 
    transform = glm::translate(glm::mat4(1.0f), position);
    transform = transform * glm::mat4_cast(rotation); 
    transform = glm::scale(transform, scale);
    return;
    transform = glm::translate(glm::mat4(1.0f), position) * (glm::mat4(1.0f) * glm::mat4_cast(rotation)) * glm::scale(glm::mat4(1.0f), scale);
}


void Object::ImGui_ObjectTransform()
{
    static std::unordered_map<void*, glm::vec3> eulerMap = std::unordered_map<void*, glm::vec3>(); 
    auto rot = PtrRotation();
    if(!eulerMap.count(rot))
        eulerMap.emplace((void*)rot, glm::degrees(glm::eulerAngles(*rot)));
        
    glm::vec3 &euler = eulerMap[rot];
    //euler = glm::eulerAngles(*rot);

    ImGui::DragFloat3("position:", &(PtrPosition()->x), 0.1f);
    ImGui::DragFloat3("rotation:", &(euler.x), 0.1f);
    ImGui::DragFloat3("scale:", &(PtrScale()->x), 0.1f);
    
    glm::quat QuatAroundX = glm::angleAxis(glm::radians(euler.x),  glm::vec3(1.0,0.0,0.0));
    glm::quat QuatAroundY = glm::angleAxis(glm::radians(euler.y), glm::vec3(0.0,1.0,0.0));
    glm::quat QuatAroundZ = glm::angleAxis(glm::radians(euler.z), glm::vec3(0.0,0.0,1.0));
    
    glm::quat finalOrientation = QuatAroundX * QuatAroundY * QuatAroundZ;
    SetRotation(finalOrientation);
    //eulerMap[rot] = euler;
}
