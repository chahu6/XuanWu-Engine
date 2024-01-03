#pragma once
#include "XuanWu/CoreUObject/Object.h"

#include <glm/ext/vector_float3.hpp>

namespace XuanWu
{
	class ActorComponent;
	class SceneComponent;

	class Actor : public Object
	{
	public:
		Actor() = default;

		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);

	public:
		glm::vec3 GetActorComponent();
		bool SetActorComponent(const glm::vec3& NewLocation);

	protected:
		Ref<SceneComponent> RootComponent;
		std::vector<Ref<ActorComponent>> OwnedComponents;
	};
}