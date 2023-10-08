#pragma once

// ���������ͷ�ļ��������ⲿ����������Щ������c�ļ�����Ľṹ��������Ҫextern"C"
extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
}

namespace XuanWu
{
	class ScriptEngine
	{
		friend class ScriptClass;
	public:
		static void Init();

		static void Shutdown();

	private:
		static void InitMono();

		static void ShutdownMono();

		static void LoadAssembly(const std::string_view filepath);

		static MonoObject* InstantiateClass(MonoClass* monoClass);
	};

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string_view classNamespace, const std::string_view className);

		MonoObject* Instantiate(); // 4.����һ����MonoClass�๹�ɵ�mono�����ҳ�ʼ��
		MonoMethod* GetMethod(const std::string_view name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
		
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;
		MonoClass* m_MonoClass = nullptr;
	};
}