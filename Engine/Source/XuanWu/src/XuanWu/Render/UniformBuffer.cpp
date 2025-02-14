#include "xwpch.h"
#include "UniformBuffer.h"

#include "XuanWu/Render/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace XuanWu
{
    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:        XW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:      return CreateRef<OpenGLUniformBuffer>(size, binding);
        }

        XW_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}

