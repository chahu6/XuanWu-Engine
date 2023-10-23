#include "xwpch.h"
#include "parser.h"
#include "Cursor/cursor.h"
#include "language_type/Class.h"
#include "Common/schema_module.h"
#include "Generator/reflection_generator.h"

#define RECURSE_NAMESPACES(kind, cursor, method, namespaces)	\
	{	\
		if (kind == CXCursor_Namespace)	\
		{	\
			auto display_name = cursor.GetDisplayName();	\
			if (!display_name.empty())	\
			{	\
				namespaces.emplace_back(display_name);	\
				method(cursor, namespaces);	\
				namespaces.pop_back();	\
			}	\
		}	\
	}

#define TRY_ADD_LANGUAGE_TYPE(handle, container)	\
	{	\
		if(handle->ShouldCompile())	\
		{	\
			auto file = handle->GetSourceFile();	\
			m_schema_modules[file].container.emplace_back(handle);	\
			m_type_table[handle->GetDisplayName()] = file;	\
		}	\
	}

MetaParser::MetaParser(const CommandInfo& info)
	:m_info(info)
{
	m_work_paths = Utils::Split(m_info.include_path, ";");

	// 反射
	m_generators.emplace_back(new Generator::ReflectionGenerator(m_work_paths[0], std::bind(&MetaParser::GetIncludeFile, this, std::placeholders::_1)));

	// @TODO 序列化

}

MetaParser::~MetaParser()
{
	for (auto item : m_generators)
	{
		if (item)
		{
			delete item;
			item = nullptr;
		}
	}

	if (m_translation_unit)
		clang_disposeTranslationUnit(m_translation_unit);

	if (m_index)
		clang_disposeIndex(m_index);
}

void MetaParser::Prepare()
{
}

int MetaParser::Parser()
{
	bool parser_include_ = ParseProjection();
	if (!parser_include_)
	{
		std::cerr << "Parsing project file error!" << std::endl;
		return -1;
	}

	std::cout << "Parsing the whole project..." << std::endl;
	int is_show_errors = Utils::ToInt(m_info.is_show_errors);
	m_index = clang_createIndex(true, is_show_errors);
	std::string pre_include = "-I";
	std::string sys_include_temp;
	if (!(m_info.sys_include == "*"))
	{
		sys_include_temp = pre_include + m_info.sys_include;
		arguments.emplace_back(sys_include_temp.c_str());
	}

	auto paths = m_work_paths;
	for (size_t index = 0; index < paths.size(); ++index)
	{
		paths[index] = pre_include + paths[index];

		arguments.emplace_back(paths[index].c_str());
	}

	fs::path input_path(m_info.source_include_file_name);
	if (!fs::exists(input_path))
	{
		std::cerr << input_path << " is not exist" << std::endl;
		return -2;
	}

	m_translation_unit = clang_createTranslationUnitFromSourceFile(
		m_index, m_info.source_include_file_name.c_str(), static_cast<int>(arguments.size()), arguments.data(), 0, nullptr);
	auto rootCursor = clang_getTranslationUnitCursor(m_translation_unit);

	Namespace tmp_namespace;

	BuildClassAST(rootCursor, tmp_namespace);

	tmp_namespace.clear();

	return 0;
}

void MetaParser::Finish()
{
	for (auto& generator_iter : m_generators)
	{
		generator_iter->Finish();
	}
}

void MetaParser::GenerateFiles()
{
	std::cout << "Start generate runtime schemas(" << m_schema_modules.size() << ")..." << std::endl;
	for (auto& schema : m_schema_modules)
	{
		for (auto& generator_iter : m_generators)
		{
			generator_iter->Generate(schema.first, schema.second);
		}
	}

	Finish();
}

bool MetaParser::ParseProjection()
{
	std::cout << "Parsing project file: " << m_info.project_input_file << std::endl;

	std::fstream include_txt_file(m_info.project_input_file, std::ios::in);

	if (include_txt_file.fail())
	{
		std::cerr << "Could not load file: " << m_info.project_input_file << std::endl;
		return false;
	}

	std::stringstream buffer;
	buffer << include_txt_file.rdbuf();

	std::string context = buffer.str();

	auto include_files = Utils::Split(context, ";");
	std::fstream include_file;

	include_file.open(m_info.source_include_file_name, std::ios::out);
	if (!include_file.is_open())
	{
		std::cerr << "Could not open the Source Include file: " << m_info.source_include_file_name << std::endl;
		return false;
	}

	std::cout << "Generating the Source Include file" << m_info.source_include_file_name << std::endl;

	std::string output_filename = Utils::GetFileName(m_info.source_include_file_name);

	if (output_filename.empty())
	{
		output_filename = "__META_INPUT_HEADER_H__";
	}
	else
	{
		Utils::Replace(output_filename, ".", "_");
		Utils::Replace(output_filename, " ", "_");
		Utils::ToUpper(output_filename);
	}

	include_file << "#ifndef __" << output_filename << "__" << std::endl;
	include_file << "#define __" << output_filename << "__" << std::endl;

	for (auto include_item : include_files)
	{
		std::string tmp_string(include_item);
		Utils::Replace(tmp_string, '\\', '/');
		include_file << "#include \"" << tmp_string << "\"" << std::endl;
	}

	include_file << "#endif" << std::endl;
	include_file.flush();
	include_file.close();

	return true;
}

void MetaParser::BuildClassAST(const Cursor& cursor, Namespace& current_namespace)
{
	for (auto& child : cursor.GetChildren())
	{
		auto kind = child.GetKind();

		if (child.IsDefinition() && (kind == CXCursor_ClassDecl || kind == CXCursor_StructDecl))
		{
			auto class_ptr = std::make_shared<Class>(child, current_namespace);

			TRY_ADD_LANGUAGE_TYPE(class_ptr, classes);
		}
		else
		{
			RECURSE_NAMESPACES(kind, child, BuildClassAST, current_namespace);
		}
	}
}

std::string MetaParser::GetIncludeFile(std::string name)
{
	return std::string();
}

void MetaParser::CollectHeaderFiles(const std::string_view source_dir, const std::string_view file)
{
	std::ofstream out_file;

	fs::path file_path = file;
	fs::path parent_path = file_path.parent_path();

	if (!fs::exists(parent_path))
	{
		fs::create_directories(parent_path);
	}

	out_file.open(file.data(), std::ios::out);
	if (!out_file.is_open())
	{
		std::cerr << "Could not open the Source Include file: " << file << std::endl;
		return;
	}

	for (const auto& entry : fs::recursive_directory_iterator(source_dir))
	{
		if (entry.path().extension() == ".h" /* || entry.path().extension() == ".hpp"*/) // 应该不会写.hpp
		{
			std::string include_string = entry.path().string();

			Utils::ReplaceAll(include_string, "\\", "/");
			out_file << include_string << ";";
		}
	}

	out_file.flush();
	out_file.close();
}
