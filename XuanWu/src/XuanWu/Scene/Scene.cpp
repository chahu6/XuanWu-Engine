#include "xwpch.h"
#include "Scene.h"
#include "glm/glm.hpp"
#include "Entity.h"

#include "XuanWu/Components/Components.h"
#include "XuanWu/Render/Renderer2D.h"
#include "ScriptableEntity.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_circle_shape.h>

namespace XuanWu
{
	static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
			case XuanWu::Rigidbody2DComponent::BodyType::Static: return b2_staticBody;
			case XuanWu::Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
			case XuanWu::Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		XW_CORE_ASSERT(false, "不支持该类型");
		return b2_staticBody;
	}

	template<typename Component>
	static void CopyComponent(entt::registry& dest, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto& view = src.view<Component>();
		for (auto e : view)
		{
			UUID uuid = src.get<IDComponent>(e).ID;

			entt::entity destEnttID = enttMap.at(uuid);
			auto& component = src.get<Component>(e);

			dest.emplace_or_replace<Component>(destEnttID, component);
		}
	}

	// 为复制实体的辅助方法
	template<typename Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		if (src.HasComponent<Component>()) 
		{
			dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}
	}

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		auto& srcSceneRegistry = other->m_Registry;
		auto& destSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		auto& view = srcSceneRegistry.view<IDComponent>();
		for (auto e : view)
		{
			UUID& uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		CopyComponent<BoxCollider2DComponent>(destSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CircleCollider2DComponent>(destSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CameraComponent>(destSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CircleRendererComponent>(destSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<NativeScriptComponent>(destSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<Rigidbody2DComponent>(destSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<SpriteRendererComponent>(destSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<TransformComponent>(destSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	Entity Scene::CreateEntity(const std::string_view name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(const UUID& uuid, const std::string_view name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);

		// 2.复制组件
		CopyComponentIfExists<TransformComponent>(newEntity, entity);
		CopyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
		CopyComponentIfExists<CircleRendererComponent>(newEntity, entity);
		CopyComponentIfExists<CameraComponent>(newEntity, entity);
		CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
		CopyComponentIfExists<Rigidbody2DComponent>(newEntity, entity);
		CopyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);
		CopyComponentIfExists<CircleCollider2DComponent>(newEntity, entity);
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(Timestep ts, const EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		// Draw Quard
		{
			auto& group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
			}
		}
		
		// Draw Circle
		{
			auto& view = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, static_cast<int>(entity));
			}
		}
		Renderer2D::EndScene();
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Script
		{
			//  [=]是隐式值捕获，捕获ts
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) 
			{
				if (!nsc.Instance) {
					// 实例化CameraController
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					// 执行CameraController脚本的OnCreate函数
					nsc.Instance->OnCreate();
				}
				
				nsc.Instance->OnUpdate(ts);
			});
		}

		// Physics
		{
			const int32_t velocityIterations = 8;
			const int32_t positionIterations = 3;
			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

			auto& view = m_Registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
		}

		// Renderer
		Camera* mainCamrea = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto[transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary)
				{
					mainCamrea = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamrea)
		{
			Renderer2D::BeginScene(*mainCamrea, cameraTransform);

			// Draw Quard
			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
					Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
				}
			}

			// Draw Circle
			{
				auto& view = m_Registry.view<TransformComponent, CircleRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
					Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, static_cast<int>(entity));
				}
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		auto& view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			// 1、主体定义
			b2BodyDef bodyDef;
			bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			// 2、由b2BodyDef创建主体
			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation); // 是否固定旋转

			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
				// 3、定义包围盒
				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				// 4、定义fixture，fixture包含定义的包围盒
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;

				// 5、定义主体的fixture
				body->CreateFixture(&fixtureDef);
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = transform.Scale.x * cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
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
				cameraComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
			}
		}
	}

	void Scene::SetFilepath(const std::string_view filepath)
	{
		m_Filepath = filepath;
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{

	}
}