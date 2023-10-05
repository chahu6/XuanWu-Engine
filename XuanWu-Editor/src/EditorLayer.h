#pragma once
#include <XuanWu.h>
#include "XuanWu/Scene/Entity.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "XuanWu/Serializer/Serializer.h"
#include "XuanWu/Render/EditorCamera.h"

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
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();

		void OnDuplicateEntity();

		void UI_Toolbar();

		void OnScenePlay();
		void OnSceneStop();

		// 编辑器层的调试
		void OnOverlayRender();
	private:
		/*Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_SpriteTexture;
		Ref<SubTexture2D> m_TextureGrass;
		
		Entity m_CameraEntity;
		Entity m_SecondCamera;
		bool m_PrimaryCamera = false;

		Entity m_SquareEntity;
		Entity m_RedSquare;
		glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
		*/

		// 必须的
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		bool m_ShowPhysicsColliders = false;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		char m_GizmoType = 7;

		Ref<XuanWu::Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportBounds[2]{};

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;

		Ref<Serializer> m_Serializer;
		EditorCamera m_EditorCamera;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		Entity m_HoveredEntity;

		Ref<Texture2D> m_IconPlay;
		Ref<Texture2D> m_IconStop;

		enum class SceneState : uint8_t
		{
			Edit = 0,
			Play = 1
		};
		SceneState m_SceneState = SceneState::Edit;
	};
}

