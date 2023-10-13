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
		// gcc��clang��ofn.lpstrFilter����LPCSTR(const char*)��ֻ��msvc��LPCWSTR(const wchar_t*)������Ҫ����һ��
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
		ofn.nFilterIndex = 1; // ����Ĭ��ѡ��ڶ����ļ����͹�����
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
		// gcc��clang��ofn.lpstrFilter����LPCSTR(const char*)��ֻ��msvc��LPCWSTR(const wchar_t*)������Ҫ����һ��
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
		ofn.nFilterIndex = 1; // ����Ĭ��ѡ��ڶ����ļ����͹�����
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