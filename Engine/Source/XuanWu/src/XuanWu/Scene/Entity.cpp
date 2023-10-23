#include "xwpch.h"
#include "Entity.h"

#include "XuanWu/Components/Components.h"

namespace XuanWu
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle), m_Scene(scene)
	{
	}

	uint64_t Entity::GetUUID()
	{
		XW_CORE_ASSERT(HasComponent<IDComponent>(), "没有该组件");

		return (uint64_t)GetComponent<IDComponent>().ID;
	}
}