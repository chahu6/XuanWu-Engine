#pragma once

#include "XuanWu/Render/RendererAPI.h"

namespace XuanWu {

	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void Clear() override;

		virtual void SetLineWidth(float width) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount = 0) override;

		// Ä£°å²âÊÔ
		virtual void SetStencilFunc(uint32_t func, int ref, uint32_t mask) override;
		virtual void SetStencilOp(uint32_t sfail, uint32_t dpfail, uint32_t dppass) override;
		virtual void SetStencilMask(uint32_t mask) override;
		virtual void SetDepthTest(bool enable) override;
	};
}