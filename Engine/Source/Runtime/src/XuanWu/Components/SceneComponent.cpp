#include "xwpch.h"
#include "SceneComponent.h"

glm::vec3 XuanWu::SceneComponent::GetComponentLocation()
{
    return Translation;
}

void XuanWu::SceneComponent::SetComponentLocation(const glm::vec3& NewLocation)
{
    Translation = NewLocation;
}
