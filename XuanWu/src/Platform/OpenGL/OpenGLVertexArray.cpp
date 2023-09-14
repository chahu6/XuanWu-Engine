#include "xwpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace XuanWu {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:			return GL_FLOAT;
		case ShaderDataType::Float2:		return GL_FLOAT;
		case ShaderDataType::Float3:		return GL_FLOAT;
		case ShaderDataType::Float4:		return GL_FLOAT;
		case ShaderDataType::Mat3:			return GL_FLOAT;
		case ShaderDataType::Mat4:			return GL_FLOAT;
		case ShaderDataType::Int:			return GL_INT;
		case ShaderDataType::Int2:			return GL_INT;
		case ShaderDataType::Int3:			return GL_INT;
		case ShaderDataType::Int4:			return GL_INT;
		case ShaderDataType::Bool:			return GL_BOOL;
		}

		XW_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		XW_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RenderID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		XW_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RenderID);
	}

	void OpenGLVertexArray::Bind() const
	{
		XW_PROFILE_FUNCTION();

		glBindVertexArray(m_RenderID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		XW_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		XW_PROFILE_FUNCTION();

		XW_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no Layout!")

		glBindVertexArray(m_RenderID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		XW_PROFILE_FUNCTION();

		glBindVertexArray(m_RenderID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}