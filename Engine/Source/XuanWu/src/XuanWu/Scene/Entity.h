#pragma once

#include <entt.hpp>
#include "Scene.h"
#include "XuanWu/Components/Components.h"

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
			XW_CORE_ASSERT(!HasComponent<T>(), "当前实体已有该组件");
			 T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle ,std::forward<Args>(args)...);
			 m_Scene->OnComponentAdded<T>(*this, component);
			 return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			XW_CORE_ASSERT(HasComponent<T>(), "当前实体没有该组件");
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
			XW_CORE_ASSERT(HasComponent<T>(), "当前实体没有该组件");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		uint64_t GetUUID();
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const{ return m_EntityHandle; }
		operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }
		bool operator==(const Entity& other) { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) { return !(*this == other); }
	private:
		entt::entity m_EntityHandle{ entt::null }; //entt的实体ID是从0开始的，所以不能初始化为0
		Scene* m_Scene = nullptr;
	};
}