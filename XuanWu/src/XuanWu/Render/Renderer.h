#pragma once

#include "RenderCommand.h"
#include "RendererAPI.h"

#include "OrthographicCamera.h"
#include "XuanWu/Camera/PerspectiveCamera.h"
#include "Shader.h"


namespace XuanWu {

	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(PerspectiveCamera& camera);
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& model = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}