#include "xwpch.h"
#include "RenderCommand.h"

namespace XuanWu {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}