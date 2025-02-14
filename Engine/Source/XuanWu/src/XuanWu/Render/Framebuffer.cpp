#include "xwpch.h"
#include "Framebuffer.h"

#include "XuanWu/Render/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace XuanWu
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: XW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
		}

		XW_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}
}