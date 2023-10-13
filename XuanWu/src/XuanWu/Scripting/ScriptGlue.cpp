#include "xwpch.h"
#include "ScriptGlue.h"
#include "XuanWu/Core/UUID.h"
#include "ScriptEngine.h"
#include "XuanWu/Scene/Scene.h"
#include "XuanWu/Scene/Entity.h"
#include "XuanWu/Core/KeyCode.h"
#include "XuanWu/Core/Input.h"

#include <glm/glm.hpp>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <box2d/b2_body.h>

namespace XuanWu
{
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

	#define XW_ADD_INTERNAL_CALL(Name) mono_add_internal_call("XuanWu.InternalCalls::" #Name, (const void*)Name) // 强制转换为(const void*)，因为其他编译器会报错，只有msvc不报错。

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
		{
			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);
			std::string managedTypename = fmt::format("XuanWu.{}", structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
			if (!managedType)
			{
				XW_CORE_ERROR("Could not find component type: {}", managedTypename);
				return;
			}
			XW_CORE_TRACE(managedTypename);

			s_EntityHasComponentFuncs[managedType] = [](Entity entity) {return entity.HasComponent<Component>(); };
		}(), ...);
	}

#pragma region Native
	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);

		mono_free(cStr);
		XW_CORE_TRACE("{},{}", str, parameter); // {}里面不写数字也可
	}

	static void NativeLog_Vector(glm::vec3* vec, glm::vec3* out)
	{
		//HZ_CORE_WARN("Value: {0}", *vec); // 这会错的，并不支持输出向量
		std::cout << vec->x << "," << vec->y << "," << vec->z << std::endl;
		//*out = glm::cross(*vec, glm::vec3(vec->x, vec->y, -vec->z)); // 通过out返回指针
		*out = glm::normalize(*vec);
	}

	static float NativeLog_VectorDot(glm::vec3* vec)
	{
		std::cout << vec->x << "," << vec->y << "," << vec->z << std::endl;
		return glm::dot(*vec, *vec);
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		XW_CORE_ASSERT(scene, "scene is nullptr");

		Entity entity = scene->GetEntityByUUID(entityID);
		XW_CORE_ASSERT(entity, "entity is nullptr");

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		XW_CORE_ASSERT(managedType, "managedType is nullptr");

		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}
#pragma endregion
	
#pragma region TransformComponent
	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		XW_CORE_ASSERT(scene, "Scene is nullptr");

		Entity entity = scene->GetEntityByUUID(entityID);
		XW_CORE_ASSERT(entity, "entity is {}");

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		XW_CORE_ASSERT(scene, "Scene is nullptr");

		Entity entity = scene->GetEntityByUUID(entityID);
		XW_CORE_ASSERT(entity, "entity is {}");

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}
#pragma endregion

#pragma region Rigidbody2DComponent
	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impluse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		XW_CORE_ASSERT(scene, "scnee is nullptr");

		Entity entity = scene->GetEntityByUUID(entityID);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		body->ApplyLinearImpulse(b2Vec2(impluse->x, impluse->y), { point->x, point->y }, wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impluse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		XW_CORE_ASSERT(scene, "scnee is nullptr");

		Entity entity = scene->GetEntityByUUID(entityID);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		body->ApplyLinearImpulseToCenter(b2Vec2(impluse->x, impluse->y), wake);
	}
#pragma endregion

#pragma region Input
	static bool Input_IsKeyDown(KeyCode keyCode)
	{
		return Input::IsKeyPressed(keyCode);
	}
#pragma endregion

	void ScriptGlue::RegisterFunctions()
	{
		XW_ADD_INTERNAL_CALL(NativeLog);
		XW_ADD_INTERNAL_CALL(NativeLog_Vector);
		XW_ADD_INTERNAL_CALL(NativeLog_VectorDot);

		XW_ADD_INTERNAL_CALL(Entity_HasComponent);

		XW_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		XW_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		XW_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		XW_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);

		XW_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}
}
