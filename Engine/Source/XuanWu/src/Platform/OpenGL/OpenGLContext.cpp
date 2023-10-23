#include "xwpch.h"

#include "OpenGLContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace XuanWu {

	OpenGLContext::OpenGLContext(GLFWwindow* window)
		:m_WindowHandle(window)
	{
		XW_CORE_ASSERT(m_WindowHandle, "Window handle is NULL!")
	}

	void OpenGLContext::Init()
	{
		XW_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		XW_CORE_ASSERT(status, "Failed to initialize Glad!");

		XW_CORE_INFO("OpenGL Info:");
		XW_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		XW_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		XW_CORE_INFO("Version: {0}", glGetString(GL_VERSION));

		//ÅÐ¶ÏOpenGL°æ±¾
		#ifdef XW_ENABLE_ASSERTS
			int versionMajor;
			int versionMinor;
			glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
			glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

			XW_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "XuanWu requires at least OpenGL version 4.5!")
		#endif // DEBUG
	}

	void OpenGLContext::SwapBuffers()
	{
		XW_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}