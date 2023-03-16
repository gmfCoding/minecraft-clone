#include "physics/staticbody.hpp"

Staticbody::Staticbody()
{
    transform = new Transform();
    position = transform->PtrPosition();
}

void Staticbody::RecalculateAABB()
{
    aabb->min = *position - (aabb->size * 0.5f);
    aabb->max = *position + (aabb->size * 0.5f);
}