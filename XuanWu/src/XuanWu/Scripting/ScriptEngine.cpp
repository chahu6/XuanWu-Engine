#include "xwpch.h"
#include "ScriptEngine.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace XuanWu
{
	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;

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

	static MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		MonoClass* klass = mono_class_from_name(image, namespaceName, className);

		if (klass == nullptr)
		{
			// 在此处记录错误
			return nullptr;
		}

		return klass;
	}

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData;
		InitMono();
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

		// 创建一个应用域
		s_Data->AppDomain = mono_domain_create_appdomain("XWAppDomain", nullptr);
		XW_CORE_ASSERT(s_Data->AppDomain, "AppDomain is nullptr");
		// 将新的应用程序域设置为当前应用程序域
		mono_domain_set(s_Data->AppDomain, true);

		// 加载C#项目导出的dll
		s_Data->CoreAssembly = LoadCSharpAssembly("Resources/Scripts/XuanWu-ScriptCore.dll");
		PrintAssemblyTypes(s_Data->CoreAssembly);

		// 加载Class
		MonoClass* MainClass = GetClassInAssembly(s_Data->CoreAssembly, "XuanWu", "Main");

		MonoObject* classInstance = mono_object_new(s_Data->AppDomain, MainClass);
		if (classInstance == nullptr)
		{
			// Log error here
		}
		// 初始化，调用无参构造函数
		mono_runtime_object_init(classInstance);

		// 调用无参函数
		MonoMethod* method = mono_class_get_method_from_name(MainClass, "PrintMessage", 0);
		if (method == nullptr)
		{
			// Log error here
		}
		MonoObject* exception = nullptr;
		mono_runtime_invoke(method, classInstance, nullptr, &exception);
		// TODO: Handle the exception

		// 调用有参函数
		MonoMethod* printInt = mono_class_get_method_from_name(MainClass, "PrintInt", 1);
		int value = 999;
		void* param = &value;
		mono_runtime_invoke(printInt, classInstance, &param, nullptr);

		// 调用多参函数
		MonoMethod* printInts = mono_class_get_method_from_name(MainClass, "PrintInts", 2);
		int a = 6;
		int b = 9;
		void* params[] = { &a, &b };
		mono_runtime_invoke(printInts, classInstance, params, nullptr);

		// 传String
		MonoMethod* printCustomMessage = mono_class_get_method_from_name(MainClass, "PrintCustomMessage", 1);
		MonoString* monoString = mono_string_new(s_Data->AppDomain, "Hello C#!!!");
		void* stringParam = monoString;
		mono_runtime_invoke(printCustomMessage, classInstance, &stringParam, nullptr);
	}

	void ScriptEngine::ShutdownMono()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;

		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}
}
