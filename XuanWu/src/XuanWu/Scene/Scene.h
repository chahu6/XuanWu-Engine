#pragma once

#include <entt.hpp>
#include "XuanWu/Core/Timestep.h"
#include "XuanWu/Render/EditorCamera.h"

namespace XuanWu
{
	class Entity;

	class Scene
	{
	public:
		friend class Entity;
		friend class SceneHierarchyPanel;

		Scene();
		~Scene();

		Entity CreateEntity(const std::string_view name = "");
		void DestroyEntity(Entity entity);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, const EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		void SetFilepath(const std::string_view filepath);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		std::string m_Filepath;
	public:
		inline entt::registry& Reg() { return m_Registry; }
		inline std::string& GetFilepath() { return m_Filepath; }
	};
}