#include "xwpch.h"
#include "Scene.h"
#include "glm/glm.hpp"

#include "Components.h"
#include "XuanWu/Render/Renderer2D.h"

namespace XuanWu
{
	static void OnTransformConstruct(entt::registry& registry, entt::entity entity) {

	}

	Scene::Scene()
	{
#if ENTT_EMPLACE_TEST
		struct TransformComponent
		{
			glm::mat4 Transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4& transform)
				:Transform(transform){}
			operator glm::mat4& () { return Transform; }
			operator const glm::mat4& () const { return Transform; }
		};

		struct MeshComponent{
			glm::mat4 Transform;
		};

		TransformComponent transform;

		// 1、创建一个实体，返回uint32_t
		entt::entity entity = m_Registry.create();

		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		if(m_Registry.any_of<TransformComponent>(entity))
			TransformComponent& t = m_Registry.get<TransformComponent>(entity);

		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform1 = m_Registry.get<TransformComponent>(entity);
			TransformComponent& transform2 = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group) {
			auto[transfrom3, mesh] = group.get<TransformComponent, MeshComponent>(entity);
			//Renderer::Submit(mesh, transform);
		}

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();
#endif
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}

	Scene::~Scene()
	{

	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}
}