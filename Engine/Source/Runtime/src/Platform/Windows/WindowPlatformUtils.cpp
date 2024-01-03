#include "xwpch.h"
#include "XuanWu/Utils/PlatformUtils.h"
#include <commdlg.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "XuanWu/Core/Application.h"

namespace XuanWu
{
	std::string FileDialogs::SaveFile(const wchar_t* filter)
	{
		OPENFILENAME ofn;
		TCHAR szFile[256] = { 0 };
		
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.hInstance = GetModuleHandle(NULL);
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
		ofn.nFilterIndex = 1; // 设置默认选择第二个文件类型过滤器
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileName(&ofn) == TRUE)
		{
			std::wstring str = ofn.lpstrFile;
			return std::string(str.begin(), str.end());
		}

		return std::string();
	}

	std::string FileDialogs::OpenFile(const wchar_t* filter)
	{
		OPENFILENAME ofn;
		TCHAR szFile[256] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL; // glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.hInstance = GetModuleHandle(NULL);
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
		ofn.nFilterIndex = 1; // 设置默认选择第二个文件类型过滤器
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileName(&ofn) == TRUE)
		{
			std::wstring str = ofn.lpstrFile;
			return std::string(str.begin(), str.end());
		}

		return std::string();
	}
}