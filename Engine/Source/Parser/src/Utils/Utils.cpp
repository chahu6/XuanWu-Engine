#include "xwpch.h"
#include "Utils.h"

#include "Cursor/cursor_type.h"

namespace Utils
{
	std::vector<std::string> Split(std::string str, const std::string_view pat)
	{
		std::vector<std::string> ret_list;

		while (true)
		{
			size_t index = str.find(pat);
			std::string sub_list = str.substr(0, index);
			if (!sub_list.empty())
			{
				ret_list.emplace_back(sub_list);
			}
			str.erase(0, index + pat.length());
			if (index == std::string::npos)
			{
				break;
			}
		}
		return ret_list;
	}

	std::string GetFileName(std::string path)
	{
		if (path.size() < 1)
		{
			return std::string{};
		}

		std::vector<std::string> result = Split(path, "/");
		if (result.size() < 1)
		{
			return std::string{};
		}
		return result[result.size() - 1];
	}

	std::string Replace(std::string& source_string, std::string sub_string, const std::string new_string)
	{
		std::string::size_type pos = 0;
		while ((pos = source_string.find(sub_string)) != std::string::npos)
		{
			source_string.replace(pos, sub_string.length(), new_string);
		}

		return source_string;
	}

	std::string Replace(std::string& source_string, char target_char, const char new_char)
	{
		std::replace(source_string.begin(), source_string.end(), target_char, new_char);
		return source_string;
	}

	void ReplaceAll(std::string& source_string, std::string sub_string, std::string new_str)
	{
		std::string::size_type pos = 0;
		while ((pos = source_string.find(sub_string)) != std::string::npos)
		{
			source_string = source_string.replace(pos, sub_string.length(), new_str);
		}
	}

	std::string ToUpper(std::string& source_string)
	{
		std::transform(source_string.begin(), source_string.end(), source_string.begin(), ::toupper);
		return source_string;
	}

	std::string Trim(std::string& source_string, const std::string trim_chars)
	{
		size_t left_pos = source_string.find_first_not_of(trim_chars);
		if (left_pos == std::string::npos)
		{
			source_string = std::string{};
		}
		else
		{
			size_t right_pos = source_string.find_last_not_of(trim_chars);
			source_string = source_string.substr(left_pos, right_pos - left_pos + 1);
		}
		return source_string;
	}

	int ToInt(const std::string_view str)
	{
		return std::stoi(str.data());
	}

	void ToString(CXString& str, std::string& output)
	{
		std::string c_str = clang_getCString(str);
		output = c_str;

		// 释放资源
		clang_disposeString(str); 
	}

	std::string GetTypeNameWithoutNamespace(const CursorType& type)
	{
		std::string&& type_name = type.GetDisplayName();
		return type_name;
	}

	std::string GetNameWithoutFirstM(const std::string_view type_name)
	{
		std::string result = type_name.data();
		if (type_name.size() > 2 && type_name[0] == 'm' && type_name[1] == '_')
		{
			result = type_name.substr(2);
		}
		return result;
	}

	std::string GetStringWithoutQuot(std::string input)
	{
		size_t left = input.find_first_of('\"') + 1;
		size_t right = input.find_last_of('\"');
		if (left > 0 && right < input.length() && left < right)
		{
			return input.substr(left, right - left);
		}
		else
		{
			return input;
		}
	}

	std::string LoadFile(std::string path)
	{
		std::ifstream iFile(path);
		std::string line_string;
		std::ostringstream template_stream;
		if (false == iFile.is_open())
		{
			iFile.close();
			return "";
		}
		while (std::getline(iFile, line_string))
		{
			template_stream << line_string << std::endl;
		}
		iFile.close();

		return template_stream.str();
	}

	fs::path MakeRelativePath(const fs::path& from, const fs::path& to)
	{
		std::string from_complete_string;
		std::string to_complete_string;

		/*remove ".." and "."*/
		FormatPathString(from.string(), from_complete_string);
		FormatPathString(to.string(), to_complete_string);

		fs::path from_complete = from_complete_string;
		fs::path to_complete = to_complete_string;

		auto iter_from = from_complete.begin();
		auto iter_to = to_complete.begin();

		// 循环，知道找到两个路径不同的分支路径，或一方到头
		while (iter_from != from_complete.end() && iter_to != to_complete.end() && (*iter_to) == (*iter_from))
		{
			++iter_from;
			++iter_to;
		}

		fs::path final_path;
		while (iter_from != from_complete.end())
		{
			final_path /= "..";
			++iter_from;
		}

		while (iter_to != to_complete.end())
		{
			final_path /= *iter_to;
			++iter_to;
		}

		return final_path;
	}

	unsigned long FormatPathString(const std::string_view path_string, std::string& out_string)
	{
		unsigned int ulRet = 0;
		std::string local_path_string = path_string.data();
		fs::path local_path;

		local_path = local_path_string;
		if (local_path.is_relative())
		{
			local_path_string = fs::current_path().string() + "/" + local_path_string;
		}

		ReplaceAll(local_path_string, "\\", "/");
		std::vector<std::string> subString = Split(local_path_string, "/");

		std::vector<std::string> out_sub_string;
		for (auto p : subString)
		{
			if (p == "..")
			{
				out_sub_string.pop_back();
			}
			else if (p != ".")
			{
				out_sub_string.push_back(p);
			}
		}

		for (size_t i = 0; i < out_sub_string.size() - 1; ++i)
		{
			out_string.append(out_sub_string[i] + "/");
		}
		// 最后一个字符串自己加
		out_string.append(out_sub_string[out_sub_string.size() - 1]);

		return 0;
	}

	std::string FormatQualifiedName(std::string& source_string)
	{
		Utils::Replace(source_string, '<', 'L');
		Utils::Replace(source_string, ':', 'S');
		Utils::Replace(source_string, '>', 'R');
		Utils::Replace(source_string, '*', 'P');
		return source_string;
	}

	std::string GetNameWithoutContainer(std::string name)
	{
		size_t left = name.find_first_of('<') + 1;
		size_t right = name.find_last_of('>');
		if (left > 0 && right < name.length() && left < right)
		{
			return name.substr(left, right - left);
		}
		else
		{
			return std::string{};
		}
	}

	void SaveFile(const std::string_view output_string, const std::string_view output_file)
	{
		fs::path out_path(output_file);

		if (!fs::exists(out_path.parent_path()))
		{
			fs::create_directories(out_path.parent_path());
		}
		std::fstream output_file_stream(output_file.data(), std::ios::out);

		output_file_stream << output_string.data() << std::endl;
		output_file_stream.flush();
		output_file_stream.close();
	}
}
