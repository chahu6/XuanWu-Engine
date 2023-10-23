#pragma once

#include "XuanWu/Render/GraphicsContext.h"

struct GLFWwindow;
namespace XuanWu {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}