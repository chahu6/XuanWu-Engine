#pragma once
#include <XuanWu.h>
#include "ParticleSystem.h"

class Sandbox2D : public XuanWu::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(XuanWu::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(XuanWu::Event& event) override;
private:
	//XuanWu::Ref<XuanWu::Shader> m_Shader;
	XuanWu::Ref<XuanWu::Texture2D> m_Texture;
	XuanWu::Ref<XuanWu::Texture2D> m_SpriteTexture;
	XuanWu::Ref<XuanWu::SubTexture2D> m_TextureGrass;

	XuanWu::OrthographicCameraController m_CameraController;
	//XuanWu::Ref<XuanWu::VertexArray> m_SquareVA;

	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };

	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem;

	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, XuanWu::Ref<XuanWu::SubTexture2D>>s_TextureMap;
};