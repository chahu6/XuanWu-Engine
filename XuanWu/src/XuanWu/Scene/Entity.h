#pragma once

#include <entt.hpp>
#include "Scene.h"

namespace XuanWu
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(entt::entity handle, Scene* scene);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			XW_CORE_ASSERT(!HasComponent<T>(), "��ǰʵ�����и����");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle ,std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			XW_CORE_ASSERT(HasComponent<T>(), "��ǰʵ��û�и����");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			XW_CORE_ASSERT(HasComponent<T>(), "��ǰʵ��û�и����");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		bool operator==(const Entity& other) { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) { return !(*this == other); }
	private:
		entt::entity m_EntityHandle{ entt::null }; //entt��ʵ��ID�Ǵ�0��ʼ�ģ����Բ��ܳ�ʼ��Ϊ0
		Scene* m_Scene = nullptr;
	};
}