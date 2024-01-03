#pragma once
#include "ActorComponent.h"
#include "glm/glm.hpp"

namespace XuanWu
{
	class SceneComponent : public ActorComponent
	{
	public:
		SceneComponent() = default;

		glm::vec3 GetComponentLocation();
		void SetComponentLocation(const glm::vec3& NewLocation);

	private:
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		SceneComponent* AttachParent;
		std::vector<SceneComponent*> AttachChildren;
	};
}