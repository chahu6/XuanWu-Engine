#include "xwpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace XuanWu {

	void OpenGLRendererAPI::Init()
	{
		XW_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);	// 深度测试
		glEnable(GL_LINE_SMOOTH);	// 将画的线条变的光滑

		glEnable(GL_STENCIL_TEST);  // 模板测试

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);// 模板测试
	}

	void OpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		// 不知道Cherno为什么要让他等于零时让他等于这个整个的数量，搞不懂
		//uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
		vertexArray->Unbind();
	}

	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRendererAPI::SetStencilFunc(uint32_t func, int ref, uint32_t mask)
	{
		glStencilFunc(func, ref, mask);
	}

	void OpenGLRendererAPI::SetStencilOp(uint32_t sfail, uint32_t dpfail, uint32_t dppass)
	{
		glStencilOp(sfail, dpfail, dppass);
	}

	void OpenGLRendererAPI::SetStencilMask(uint32_t mask)
	{
		glStencilMask(mask);
	}

	void OpenGLRendererAPI::SetDepthTest(bool enable)
	{
		enable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}
}
