#include "xwpch.h"
#include "ScriptEngine.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "ScriptGlue.h"

namespace XuanWu
{
	namespace Utils
	{
		static char* ReadBytes(const std::string_view filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath.data(), std::ios::binary | std::ios::ate);

			if (!stream)
			{
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if (size == 0)
			{
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}

		static MonoAssembly* LoadCSharpAssembly(const std::string_view assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, false);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.data(), &status, false);
			mono_image_close(image);

			delete[] fileData;

			return assembly;
		}

		static void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				XW_CORE_TRACE("{0}.{1}", nameSpace, name);
			}
		}
	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		ScriptClass EntityClass;
	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData;

		// 1、初始化mono
		InitMono();

		// 2、加载c#程序集
		LoadAssembly("Resources/Scripts/XuanWu-ScriptCore.dll");

		// 添加内部调用
		ScriptGlue::RegisterFunctions();

		// 3、创建一个MonoClass类
		s_Data->EntityClass = ScriptClass("XuanWu", "Main");

		// 4、创建一个Main类构成的mono对象并初始化
		MonoObject* instance = s_Data->EntityClass.Instantiate();

		// 5.1、调用Main类的函数-无参
		MonoMethod* printMessageFunc = s_Data->EntityClass.GetMethod("PrintMessage", 0);
		s_Data->EntityClass.InvokeMethod(instance, printMessageFunc);

		// 5.2、调用Main类的函数-带参
		MonoMethod* printIntFunc = s_Data->EntityClass.GetMethod("PrintInt", 1);
		int value = 5;
		void* param = &value;
		s_Data->EntityClass.InvokeMethod(instance, printIntFunc, &param);

		MonoMethod* printIntsFunc = s_Data->EntityClass.GetMethod("PrintInts", 2);
		int x = 6;
		int y = 9;
		void* params[] ={
			&x, &y
		};
		s_Data->EntityClass.InvokeMethod(instance, printIntsFunc, params);

		// 5.3、调用带string函数
		MonoMethod* printCustomMessageFunc = s_Data->EntityClass.GetMethod("PrintCustomMessage", 1);
		MonoString* str = mono_string_new(s_Data->AppDomain, "Hello World from CPP!!!");
		void* strParam = str;
		s_Data->EntityClass.InvokeMethod(instance, printCustomMessageFunc, &strParam);
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		if (s_Data != nullptr)
		{
			delete s_Data;
			s_Data = nullptr;
		}
	}

	void ScriptEngine::InitMono()
	{
		// 设置程序集装配路径(复制的4.5版本的路径)
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("XuanWuRuntime");
		XW_CORE_ASSERT(rootDomain, "RootDomain is nullptr");

		s_Data->RootDomain = rootDomain;
	}

	void ScriptEngine::ShutdownMono()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;

		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}

	void ScriptEngine::LoadAssembly(const std::string_view filepath)
	{
		// 创建一个应用域
		s_Data->AppDomain = mono_domain_create_appdomain("XWScriptRuntime", nullptr);
		XW_CORE_ASSERT(s_Data->AppDomain, "AppDomain is nullptr");

		// 将新的应用程序域设置为当前应用程序域
		mono_domain_set(s_Data->AppDomain, true);

		// 加载C#项目导出的dll
		s_Data->CoreAssembly = Utils::LoadCSharpAssembly(filepath);
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		Utils::PrintAssemblyTypes(s_Data->CoreAssembly); // 打印dll的基本信息
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);

		if (instance == nullptr)
			return nullptr;

		mono_runtime_object_init(instance);
		return instance;
	}

	// ScriptClass
	ScriptClass::ScriptClass(const std::string_view classNamespace, const std::string_view className)
		:m_ClassNamespace(className.data()), m_ClassName(className.data())
	{
		m_MonoClass = mono_class_from_name(s_Data->CoreAssemblyImage, classNamespace.data(), className.data());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string_view name, int parameterCount)
	{
		MonoMethod* method = mono_class_get_method_from_name(m_MonoClass, name.data(), parameterCount);

		return method;
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}
}
