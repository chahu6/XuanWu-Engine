#include "xwpch.h"
#include "parser/parser.h"

int Parse(const CommandInfo& info);

/**
*	argv[1] : Ԥ�������Ѽ���ͷ�ļ�·��������ļ���
*	argv[2] : �����Ѽ���ͷ�ļ�·���������İ�������·����ͷ�ļ�
*	argv[3] : ��Ҫ�Ѽ������Դ�ļ�·��
*	argv[4] : �Ƿ���������ļ��аɣ�Ӧ���ǰ�
*	argv[5] : �����ڵ������ռ�
*	argv[6] : �Ƿ��ӡ������Ϣ
*/

int main(int argc, char* argv[])
{
	/*argv[1] = "D:/Test-C++/XuanWu/Engine/Intermediate/precompile.json";
	argv[2] = "D:/Test-C++/XuanWu/Engine/Intermediate/parser_header.h";
	argv[3] = "D:/Test-C++/XuanWu/Engine/Source/XuanWu/src";
	argv[4] = "*";
	argv[5] = "XuanWu";
	argv[6] = "0";*/

	auto start_time = std::chrono::system_clock::now();
	int result = 0;


	if (argv[1] != nullptr && argv[2] != nullptr && argv[3] != nullptr &&
		argv[4] != nullptr && argv[5] != nullptr && argv[6] != nullptr)
	{
		CommandInfo info{ argv[1], argv[2], argv[3], argv[4], argv[5], argv[6] };

		MetaParser::CollectHeaderFiles(info.include_path, info.project_input_file);

		MetaParser::Prepare();

		result = Parse(info);
		if (result != 0)
		{
			assert(false);
		}

		auto duration_time = std::chrono::system_clock::now() - start_time;
		std::cout << "Completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(duration_time).count() << "ms" << std::endl;
		return result;
	}
	else
	{
		std::cerr << "Arguments parser error !" << std::endl;

		return -1;
	}

	return 0;
}

int Parse(const CommandInfo& info)
{
	std::cout << std::endl;
	std::cout << "Parsing meta data fro starget \"" << info.module_name << "\"" << std::endl;

	bool is_show_errors = "0" != info.is_show_errors; 

	MetaParser parser(info);

	std::cout << "Parsing in " << info.include_path << std::endl;
	int result = parser.Parser();
	if (0 != result)
	{
		return result;
	}

	parser.GenerateFiles();

	return 0;
}