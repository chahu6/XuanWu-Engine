#include "xwpch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace XuanWu {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case XuanWu::RendererAPI::API::None:	XW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case XuanWu::RendererAPI::API::OpenGL:	return CreateScope<OpenGLRendererAPI>();
		}

		XW_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}