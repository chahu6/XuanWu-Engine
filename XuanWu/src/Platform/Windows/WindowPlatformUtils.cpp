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
		// gcc和clang的ofn.lpstrFilter都是LPCSTR(const char*)，只有msvc是LPCWSTR(const wchar_t*)，所以要适配一下
#ifdef _MSC_VER
		ofn.lpstrFilter = filter;
#elif defined(__GNUC__)
		int bufferSize = wcstombs(nullptr, filter, 0);
		char* narrowFilter = new char[bufferSize];
		wcstombs(narrowFilter, filter, bufferSize);
		ofn.lpstrFilter = narrowFilter;
#endif
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
		ofn.nFilterIndex = 1; // 设置默认选择第二个文件类型过滤器
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileName(&ofn) == TRUE)
		{
#ifdef _MSC_VER
			std::wstring str = ofn.lpstrFile;
			return std::string(str.begin(), str.end());
#elif defined(__GNUC__)
			return ofn.lpstrFile;
#endif
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
		// gcc和clang的ofn.lpstrFilter都是LPCSTR(const char*)，只有msvc是LPCWSTR(const wchar_t*)，所以要适配一下
#ifdef _MSC_VER
		ofn.lpstrFilter = filter;
#elif defined(__GNUC__)
		int bufferSize = wcstombs(nullptr, filter, 0);
		char* narrowFilter = new char[bufferSize];
		wcstombs(narrowFilter, filter, bufferSize);
		ofn.lpstrFilter = narrowFilter;
#endif
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
		ofn.nFilterIndex = 1; // 设置默认选择第二个文件类型过滤器
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileName(&ofn) == TRUE)
		{
#ifdef _MSC_VER
			std::wstring str = ofn.lpstrFile;
			return std::string(str.begin(), str.end());
#elif defined(__GNUC__)
			return ofn.lpstrFile;
#endif
		}

		return std::string();
	}
}