#pragma once
#include <XuanWu.h>
#include "XuanWu/Scene/Entity.h"
#include "Panels/SceneHierarchyPanel.h"

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
	protected:
		bool OnWindowResized(WindowResizeEvent& event);
	private:
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_SpriteTexture;
		Ref<SubTexture2D> m_TextureGrass;

		glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		Ref<XuanWu::Framebuffer> m_Framebuffer;
		OrthographicCameraController m_CameraController;

		Ref<Scene> m_ActiveScene;
		SceneHierarchyPanel m_SceneHierarchyPanel;

		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;
		bool m_PrimaryCamera = false;
	};
}

