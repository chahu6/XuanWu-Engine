#include "xwpch.h"
#include "Actor.h"

#include "Components/SceneComponent.h"

void XuanWu::Actor::BeginPlay()
{
	for (auto& Component : OwnedComponents)
	{
		Component->BeginPlay();
	}
}

void XuanWu::Actor::Tick(float DeltaTime)
{
	for (auto& Component : OwnedComponents)
	{
		Component->TickComponent(DeltaTime);
	}
}

glm::vec3 XuanWu::Actor::GetActorComponent()
{
	return RootComponent != nullptr ? RootComponent->GetComponentLocation() : glm::vec3(0.0f);
}

bool XuanWu::Actor::SetActorComponent(const glm::vec3& NewLocation)
{
	if (RootComponent)
	{
		RootComponent->SetComponentLocation(NewLocation);
		return true;
	}

	return false;
}
