#ifndef __PARSER_H__
#define __PARSER_H__

#include "Generator/generator.h"


struct CommandInfo
{
	std::string		project_input_file;
	std::string		source_include_file_name;		// 输出的文件名
	std::string		include_path;
	std::string		sys_include;
	std::string		module_name;
	std::string		is_show_errors;
};

class Cursor;

class MetaParser
{
	typedef std::vector<std::string> Namespace;
public:
	MetaParser(const CommandInfo& info);
	~MetaParser();

	static void Prepare();
	static void CollectHeaderFiles(const std::string_view source_dir, const std::string_view file);

	int Parser();
	void Finish();
	void GenerateFiles();

private:
	bool ParseProjection();
	void BuildClassAST(const Cursor& cursor, Namespace& current_namespace);
	std::string GetIncludeFile(std::string name);


private:
	CommandInfo m_info;

	std::vector<const char*> arguments = { "-x", "c++",
			"-std=c++11",
			"-D__REFLECTION_PARSER__",
			"-DNDEBUG",
			"-D__clang__",
			"-w",
			"-MG",
			"-M",
			"-ferror-limit=0",
			"-o clangLog.txt"
	};

	CXIndex m_index;
	CXTranslationUnit m_translation_unit;

	std::unordered_map<std::string, std::string> m_type_table;
	std::unordered_map<std::string, SchemaModule> m_schema_modules;

	std::vector<std::string> m_work_paths;
	std::vector<Generator::GeneratorInterface*> m_generators;
};

#endif // !_PARSER_H
