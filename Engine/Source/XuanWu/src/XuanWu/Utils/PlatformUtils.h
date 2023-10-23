#pragma once

#include <string>

namespace XuanWu
{
	class FileDialogs
	{
	public:
		static std::string SaveFile(const wchar_t* filter);

		static std::string OpenFile(const wchar_t* filter);
	};
}