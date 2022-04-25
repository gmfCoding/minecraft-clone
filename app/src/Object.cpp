#include "Object.hpp"
#include <string>
#include <glm/gtc/type_ptr.hpp>

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
    transform = glm::mat4(1.0f) * glm::mat4_cast(rotation); 
    transform = glm::scale(transform, scale);
    transform = glm::translate(transform, position);
}
