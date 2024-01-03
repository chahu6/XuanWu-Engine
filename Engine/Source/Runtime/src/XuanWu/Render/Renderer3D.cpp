#include "xwpch.h"
#include "Renderer3D.h"

#include <glm/glm.hpp>
#include "VertexArray.h"
#include "EditorCamera.h"
#include "UniformBuffer.h"
#include "Shader.h"

#include "XuanWu/3D/Model.h"

namespace XuanWu 
{
	struct Renderer3DData
	{
		// Draw Model
		Ref<Shader> ModelShader;

		Renderer3D::Statistics Stats;

		// uniform缓冲对象，存储摄像机的视图矩阵
		struct CameraData
		{
			glm::mat4 ViewProjection;
		};

		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		XW_PROFILE_FUNCTION();

		// uniform缓冲对象
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);

		// Shader
		s_Data.ModelShader = Shader::Create("Renderer3D_Model", "assets/shaders/Renderer3D_Model.vs", "assets/shaders/Renderer3D_Model.frag");
	}

	void Renderer3D::Shutdown()
	{
		XW_PROFILE_FUNCTION();

	}

	void Renderer3D::BeginScene(const EditorCamera& camera)
	{
		XW_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));

		StartBatch();
	}

	void Renderer3D::EndScene()
	{
	}

	void Renderer3D::DrawModel(const glm::mat4& transform, const glm::vec2& size, Model* model, int entityID)
	{
		if (model == nullptr) return;

		s_Data.ModelShader->Bind();
		s_Data.ModelShader->SetMat4("u_Transform", transform);
		s_Data.ModelShader->SetInt("u_EntityID", entityID);

		model->Draw(s_Data.ModelShader);
	}

	void Renderer3D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer3D::Statistics Renderer3D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer3D::StartBatch()
	{

	}

	void Renderer3D::NextBatch()
	{
		EndScene();

		StartBatch();
	}
}