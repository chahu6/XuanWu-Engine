#pragma once

// 如果不引入头文件，必须外部声明，但这些都是在c文件定义的结构，所以需要extern"C"
extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace XuanWu
{
	class Scene;
	class Entity;
	class Timestep;

	enum class ScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool, Char, Byte, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity
	};

	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;

		MonoClassField* ClassField;
	};

	class ScriptClass
	{
		friend class ScriptEngine;
	public:
		ScriptClass() = default;
		ScriptClass(const std::string_view classNamespace, const std::string_view className, bool bIsCore = false);

		MonoObject* Instantiate(); // 4.创建一个由MonoClass类构成的mono对象并且初始化
		MonoMethod* GetMethod(const std::string_view name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		inline const std::map<std::string, ScriptField>& GetFields() const { return m_Fields; }
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;
		MonoClass* m_MonoClass = nullptr;

		std::map<std::string, ScriptField> m_Fields;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(uint64_t uuid, Ref<ScriptClass> scriptClass);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);

		Ref<ScriptClass> GetScriptClass() { return m_ScriptClass; }

		template<typename T>
		T GetFieldValue(const std::string_view field)
		{
			bool bSuccess = GetFieldValueInternal(field, s_FieldValueBuffer);
			if (!bSuccess)
				return T{};
			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string_view field, T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");

			SetFieldValueInternal(field, &value);
		}
	private:
		bool GetFieldValueInternal(const std::string_view name, void* buffer);
		bool SetFieldValueInternal(const std::string_view name, const void* value);

	private:
		uint64_t m_EntityUUID;
		Ref<ScriptClass> m_ScriptClass;

		inline static char s_FieldValueBuffer[16];

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

		static Ref<ScriptInstance> GetEntityScriptInstance(Entity entity);
	private:
		static void InitMono();

		static void ShutdownMono();

		static void LoadCoreAssembly(const std::string_view filepath);

		static void LoadAppAssembly(const std::string_view filepath);

		static void LoadAssemblyClasses();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
	};
}