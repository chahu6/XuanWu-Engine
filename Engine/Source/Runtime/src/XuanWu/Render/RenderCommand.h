#pragma once

#include "RendererAPI.h"

namespace XuanWu {

	class RenderCommand 
	{
	public:
		inline static void Init()
		{
			XW_PROFILE_FUNCTION();

			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount = 0)
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}

		inline static void SetLineWidth(float width)
		{
			s_RendererAPI->SetLineWidth(width);
		}

		inline static void SetStencilFunc(uint32_t func, int ref, uint32_t mask)
		{
			s_RendererAPI->SetStencilFunc(func, ref, mask);
		}

		inline static void SetStencilOp(uint32_t sfail, uint32_t dpfail, uint32_t dppass)
		{
			s_RendererAPI->SetStencilOp(sfail, dpfail, dppass);
		}

		inline static void SetStencilMask(uint32_t mask)
		{
			s_RendererAPI->SetStencilMask(mask);
		}

		inline static void SetDepthTest(bool enable)
		{
			s_RendererAPI->SetDepthTest(enable);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}