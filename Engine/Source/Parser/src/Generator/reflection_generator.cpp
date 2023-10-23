#include "xwpch.h"
#include "reflection_generator.h"
#include "Common/schema_module.h"
#include "language_type/class.h"
#include "language_type/field.h"
#include "template_manager/template_manager.h"

namespace Generator
{
	ReflectionGenerator::ReflectionGenerator(std::string source_directory, std::function<std::string(std::string)> get_include_function)
		:GeneratorInterface(source_directory + REFLECTION_PATH, source_directory, get_include_function)
	{
		PrepareStatus(m_out_path);
	}

	int ReflectionGenerator::Generate(std::string path, SchemaModule schema)
	{
		static const std::string vector_prefix = "std::vector<";
		std::string file_path = ProcessFileName(path); // @TODO 这里生成reflection.gen.h名称

		Mustache::data mustache_data;
		Mustache::data include_headfiles(Mustache::data::type::list);
		Mustache::data class_defines(Mustache::data::type::list);

		std::string include_headfile = Utils::MakeRelativePath(m_root_path, path).string();
		Utils::ReplaceAll(include_headfile, "\\", "/");
		include_headfiles.push_back(
			Mustache::data("headfile_name", include_headfile));

		std::map<std::string, bool> class_names;
		for (auto class_tmp : schema.classes)
		{
			if (!class_tmp->ShouldCompile())
				continue;

			// 明明直接true就行，为什么分两步，看不懂
			class_names.insert_or_assign(class_tmp->GetClassName(), false);
			class_names[class_tmp->GetClassName()] = true;

			std::vector<std::string> field_names;
			std::map<std::string, std::pair<std::string, std::string>> vector_map;

			Mustache::data class_def;
			Mustache::data vector_defines(Mustache::data::type::list);

			GenClassRenderData(class_tmp, class_def);
			for (auto field : class_tmp->GetFields())
			{
				if (!field->ShouldCompile())
					continue;
				field_names.emplace_back(field->GetName());
				bool is_array = field->GetTypeName().find(vector_prefix) == 0;
				if (is_array)
				{
					std::string array_useful_name = field->GetTypeName();
					// 不知道在干什么
					Utils::FormatQualifiedName(array_useful_name);

					std::string item_type = field->GetTypeName();

					item_type = Utils::GetNameWithoutContainer(item_type);

					vector_map[field->GetTypeName()] = std::make_pair(array_useful_name, item_type);
				}
			}

			if (vector_map.size() > 0)
			{
				if (nullptr == class_def.get("vector_exist"))
				{
					class_def.set("vector_exist", true);
				}
				for (auto vector_item : vector_map)
				{
					std::string array_useful_name = vector_item.second.first;
					std::string item_type = vector_item.second.second;
					Mustache::data vector_define;
					vector_define.set("vector_useful_name", array_useful_name);
					vector_define.set("vector_type_name", vector_item.first);
					vector_define.set("vector_element_type_name", item_type);
					vector_defines.push_back(vector_define);
				}
			}
			class_def.set("vector_defines", vector_defines);
			class_defines.push_back(class_def);
		}
		mustache_data.set("class_defines", class_defines);
		mustache_data.set("include_headfiles", include_headfiles);
		std::string render_string = TemplateManager::GetInstance()->RenderByTemplate("commonReflectionFile", mustache_data);
		Utils::SaveFile(render_string, file_path);

		for (auto class_item : class_names)
		{
			// 存的类名
			m_type_list.emplace_back(class_item.first);
		}

		// 头文件名
		std::string head_string = Utils::MakeRelativePath(m_root_path, file_path).string();
		Utils::ReplaceAll(head_string, "\\", "/");
		m_head_file_list.emplace_back(head_string);
		return 0;
	}

	void ReflectionGenerator::Finish()
	{
		Mustache::data mustache_data;
		Mustache::data include_headfiles = Mustache::data::type::list;
		Mustache::data class_defines	 = Mustache::data::type::list;

		for (auto& head_file : m_head_file_list)
		{
			include_headfiles.push_back(Mustache::data("headfile_name", head_file));
		}
		for (auto& class_name : m_type_list)
		{
			class_defines.push_back(Mustache::data("class_name", class_name));
		}
		mustache_data.set("include_headfiles", include_headfiles);
		mustache_data.set("class_defines", class_defines);
		std::string render_string = TemplateManager::GetInstance()->RenderByTemplate("allReflectionFile", mustache_data);
		Utils::SaveFile(render_string, m_out_path + "/all_reflection.h");
	}

	void ReflectionGenerator::PrepareStatus(std::string path)
	{
		GeneratorInterface::PrepareStatus(path);

		TemplateManager::GetInstance()->LoadTemplates(m_root_path, "commonReflectionFile");
		TemplateManager::GetInstance()->LoadTemplates(m_root_path, "allReflectionFile");
	}

	std::string ReflectionGenerator::ProcessFileName(std::string path)
	{
		auto relativeDir = fs::path(path).filename().replace_extension("reflection.gen.h").string();
		return m_out_path + "/" + relativeDir;
	}
}
