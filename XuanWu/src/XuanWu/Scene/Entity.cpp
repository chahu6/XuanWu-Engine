#include "xwpch.h"

#include "Entity.h"

namespace XuanWu
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle), m_Scene(scene)
	{

	}
}