#ifndef __UTILS_H__
#define __UTILS_H__

class CursorType;

namespace Utils
{
	std::vector<std::string> Split(std::string str, const std::string_view pat);

	std::string GetFileName(std::string path);

	std::string Replace(std::string& source_string, std::string sub_string, const std::string new_string);

	std::string Replace(std::string& source_string, char target_char, const char new_char);

	void ReplaceAll(std::string& source_string, std::string sub_string, std::string new_str);

	std::string ToUpper(std::string& source_string);

	std::string Trim(std::string& source_string, const std::string trim_chars);

	int ToInt(const std::string_view str);

	void ToString(CXString& str, std::string& output);

	std::string GetTypeNameWithoutNamespace(const CursorType& type);

	std::string GetNameWithoutFirstM(const std::string_view type_name);

	std::string GetStringWithoutQuot(std::string input);

	std::string LoadFile(std::string path);

	fs::path MakeRelativePath(const fs::path& from, const fs::path& to);

	unsigned long FormatPathString(const std::string_view path_string, std::string& out_string);

	std::string FormatQualifiedName(std::string& source_string);

	std::string GetNameWithoutContainer(std::string name);

	void SaveFile(const std::string_view output_string, const std::string_view output_file);
}
#endif