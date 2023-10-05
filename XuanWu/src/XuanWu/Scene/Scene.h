#pragma once
#include <entt.hpp>
#include "XuanWu/Core/Timestep.h"
#include "XuanWu/Render/EditorCamera.h"
#include "XuanWu/Core/UUID.h"

class b2World;

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

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string_view name = "");
		Entity CreateEntityWithUUID(const UUID& uuid, const std::string_view name = "");

		void DestroyEntity(Entity entity);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, const EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		// �������ʱ�ͽ���ʱ����
		void OnRuntimeStart();
		void OnRuntimeStop();

		void SetFilepath(const std::string_view filepath);

		Entity GetPrimaryCameraEntity();

		//����Entity
		void DuplicateEntity(Entity entity);

		template<typename... Component>
		auto GetAllComponentView()
		{
			return m_Registry.view<Component...>();
		}
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		// scene�������ļ�·��
		std::string m_Filepath;

		// ��������
		b2World* m_PhysicsWorld = nullptr;
	public:
		inline entt::registry& Reg() { return m_Registry; }
		inline std::string& GetFilepath() { return m_Filepath; }
	};
}