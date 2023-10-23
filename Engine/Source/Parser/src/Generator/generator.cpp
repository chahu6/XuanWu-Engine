#include "xwpch.h"
#include "generator.h"

#include "language_type/class.h"
#include "language_type/field.h"

namespace Generator
{
	GeneratorInterface::GeneratorInterface(std::string out_path, std::string root_path, std::function<std::string(std::string)> get_include_func)
		:m_out_path(out_path), m_root_path(root_path), m_get_include_func(get_include_func)
	{
	}

	void GeneratorInterface::PrepareStatus(std::string path)
	{
		if (!fs::exists(path))
		{
			fs::create_directories(path);
		}
	}

	void GeneratorInterface::GenClassRenderData(std::shared_ptr<Class> class_temp, Mustache::data& class_def)
	{
		class_def.set("class_name", class_temp->GetClassName());
		class_def.set("class_base_class_size", std::to_string(class_temp->GetBaseClasses().size()));
		class_def.set("class_need_register", true);

		if (class_temp->GetBaseClasses().size() > 0)
		{
			Mustache::data class_base_class_defines(Mustache::data::type::list);
			class_def.set("class_has_base", true);
			for (size_t index = 0; index < class_temp->GetBaseClasses().size(); ++index)
			{
				Mustache::data class_base_class_def;
				class_base_class_def.set("class_base_class_name", class_temp->GetBaseClasses().at(index)->name);
				class_base_class_def.set("class_base_class_index", std::to_string(index));
				class_base_class_defines.push_back(class_base_class_def);
			}
			class_def.set("class_base_class_defines", class_base_class_defines);
		}

		Mustache::data class_field_defines = Mustache::data::type::list;
		GenClassFieldRenderData(class_temp, class_field_defines);
		class_def.set("class_field_defines", class_field_defines);
	}

	void GeneratorInterface::GenClassFieldRenderData(std::shared_ptr<Class> class_temp, Mustache::data& field_defs)
	{
		static const std::string vector_prefix = "std::vector<";

		for (auto& field : class_temp->GetFields())
		{
			if (!field->ShouldCompile())
				continue;

			Mustache::data field_defines;

			field_defines.set("class_field_name", field->GetName());
			field_defines.set("class_field_type", field->GetTypeName());
			field_defines.set("class_field_display_name", field->GetDisplayName());
			// 对std::vector特殊处理
			bool is_vector = field->GetTypeName().find(vector_prefix) == 0;
			field_defines.set("class_field_is_vector", is_vector);
			field_defs.push_back(field_defines);
		}
	}
}