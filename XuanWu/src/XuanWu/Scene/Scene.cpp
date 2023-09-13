#include "xwpch.h"
#include "Scene.h"
#include "glm/glm.hpp"
#include "Entity.h"

#include "Components.h"
#include "XuanWu/Render/Renderer2D.h"

namespace XuanWu
{
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

	Entity Scene::CreateEntity(const std::string_view name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		Camera* mainCamrea = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary)
				{
					mainCamrea = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamrea)
		{
			Renderer2D::BeginScene(*mainCamrea, *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform, sprite.Color);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	Scene::~Scene()
	{
	}
}