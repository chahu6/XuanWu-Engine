#include "xwpch.h"
#include "FStringUtils.h"

namespace XuanWu
{
	void FStringUtils::ReplaceAll(std::string& source_str, std::string sub_str, std::string new_str)
	{
		std::string::size_type pos = 0;
		while ((pos = source_str.find(sub_str)) != std::string::npos)
		{
			source_str = source_str.replace(pos, sub_str.length(), new_str);
		}
	}
}