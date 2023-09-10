#pragma once
#include <XuanWu.h>

namespace XuanWu
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;
	private:
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_SpriteTexture;
		Ref<SubTexture2D> m_TextureGrass;

		glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };

		uint32_t m_MapWidth, m_MapHeight;

		Ref<XuanWu::Framebuffer> m_Framebuffer;
		OrthographicCameraController m_CameraController;
	};
}

