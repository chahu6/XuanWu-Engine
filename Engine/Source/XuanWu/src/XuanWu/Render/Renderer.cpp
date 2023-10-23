#include "xwpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer2D.h"

namespace XuanWu {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		XW_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& model)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setMat4("u_ViewProjectionMatrix", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setMat4("u_Model", model);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}