#pragma once

#include "XuanWu/Render/Framebuffer.h"
namespace XuanWu
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spce);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

		void Invalidate();
	private:
		uint32_t m_RendererID, m_ColorAttachment, m_DepthAttachment;
		FramebufferSpecification m_Specification;
	};
}