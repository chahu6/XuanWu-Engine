#pragma once

// 如果不引入头文件，必须外部声明，但这些都是在c文件定义的结构，所以需要extern"C"
extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoImage MonoImage;
}

namespace XuanWu
{
	class Scene;
	class Entity;
	class Timestep;

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string_view classNamespace, const std::string_view className, bool bIsCore = false);

		MonoObject* Instantiate(); // 4.创建一个由MonoClass类构成的mono对象并且初始化
		MonoMethod* GetMethod(const std::string_view name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;
		MonoClass* m_MonoClass = nullptr;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(uint64_t uuid, Ref<ScriptClass> scriptClass);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);
	private:
		uint64_t m_EntityUUID;
		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMehtod = nullptr;
	};

	class ScriptEngine
	{
		friend class ScriptClass;
		friend class ScriptGlue;
	public:
		static void Init();

		static void Shutdown();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool EntityClassExists(const std::string_view className);

		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, Timestep ts);

		static const std::unordered_map<std::string, Ref<ScriptClass>>& GetEntityClasses();

		static Scene* GetSceneContext();

		static MonoImage* GetCoreAssemblyImage();
	private:
		static void InitMono();

		static void ShutdownMono();

		static void LoadCoreAssembly(const std::string_view filepath);

		static void LoadAppAssembly(const std::string_view filepath);

		static void LoadAssemblyClasses();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
	};
}