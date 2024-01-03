#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace XuanWu {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

		virtual ~RendererAPI() = default;
	public:
		virtual void Init() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount = 0) = 0;

		virtual void SetLineWidth(float width) = 0;

		// Ä£°å²âÊÔ
		virtual void SetStencilFunc(uint32_t func, int ref, uint32_t mask) = 0;
		virtual void SetStencilOp(uint32_t sfail, uint32_t dpfail, uint32_t dppass) = 0;
		virtual void SetStencilMask(uint32_t mask) = 0;
		virtual void SetDepthTest(bool enable) = 0;

		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	public:
		inline static API GetAPI() { return s_API; }
	};
}