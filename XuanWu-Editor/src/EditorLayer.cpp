#include "xwpch.h"
#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "XuanWu/Render/Renderer2D.h"

#include "XuanWu/Utils/PlatformUtils.h"

namespace XuanWu
{
	EditorLayer::EditorLayer()
		:Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		XW_PROFILE_FUNCTION();

		m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_SpriteTexture = Texture2D::Create("assets/games/RPGpack_sheet_2X.png");

		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);

		m_ActiveScene = CreateRef<Scene>();

#if 0
		m_SquareEntity = m_ActiveScene->CreateEntity(TXT("������"));
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.f, 0.f, 1.0f, 1.0f));

		m_RedSquare = m_ActiveScene->CreateEntity(TXT("�췽��"));
		m_RedSquare.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.f, 1.f));

		m_SecondCamera = m_ActiveScene->CreateEntity(TXT("�������"));
		m_SecondCamera.AddComponent<CameraComponent>();
		m_SecondCamera.GetComponent<CameraComponent>().Primary = false;

		m_CameraEntity = m_ActiveScene->CreateEntity(TXT("�����"));
		m_CameraEntity.AddComponent<CameraComponent>();

		class CameraController : public ScriptableEntity
		{
		public:
			virtual void OnCreate() override
			{
				auto& transform = GetComponent<TransformComponent>().Translation;
				transform.x = rand() % 10 - 5;
			}

			virtual void OnDestroy() override
			{
			}

			virtual void OnUpdate(Timestep ts) override
			{
				auto& transform = GetComponent<TransformComponent>().Translation;

				float speed = 5.0f;
				if (Input::IsKeyPressed(XW_KEY_W))
					transform.y += speed * ts;
				else if (Input::IsKeyPressed(XW_KEY_S))
					transform.y -= speed * ts;
				if (Input::IsKeyPressed(XW_KEY_A))
					transform.x -= speed * ts;
				else if (Input::IsKeyPressed(XW_KEY_D))
					transform.x += speed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

#endif

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_Serializer = Serializer::Create(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		XW_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		XW_PROFILE_FUNCTION();

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && 
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// ����Statsͳ�Ƶ�����
		Renderer2D::ResetStats();

		// ��֡����
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
		RenderCommand::Clear();

		m_ActiveScene->OnUpdate(ts);

		// ���֡�����
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		XW_PROFILE_FUNCTION();

		//���������Ⱦ
		m_SceneHierarchyPanel.OnImGuiRender();

		// �˵���
		if (ImGui::BeginMainMenuBar())
		{
			// ���һ���˵�
			if (ImGui::BeginMenu(TXT("File")))
			{
				// ��Ӳ˵���
				if (ImGui::MenuItem(TXT("New", "Ctrl+N")))
					NewScene();

				if (ImGui::MenuItem(TXT("Open Level", "Ctrl+O")))
					OpenScene();

				if (ImGui::MenuItem(TXT("Save Level", "Ctrl+S")))
					SaveScene();

				if (ImGui::MenuItem(TXT("Exit")))
					Application::Get().Close();

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::Begin(TXT("Settings"));
		{
			auto stats = Renderer2D::GetStats();
			ImGui::Text(TXT("Renderer2D Stats: "));
			ImGui::Text(TXT("���ƴ�����%d"), stats.DrawCalls);
			ImGui::Text(TXT("�ı���������%d"), stats.QuadCount);
			ImGui::Text(TXT("����������%d"), stats.GetTotalVertexCount());
			ImGui::Text(TXT("����������%d"), stats.GetTotalIndexCount());
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(TXT("Viewport"));
		{
			m_ViewportFocused = ImGui::IsWindowFocused();//  �۽�Ϊtrue
			m_ViewportHovered = ImGui::IsWindowHovered();// ��ͣΪtrue
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
			{
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			}
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
		ImGui::Begin(TXT("DepthTexture"));
		{
			uint32_t textureID = m_Framebuffer->GetDepthAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_Serializer->SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile(L"XuanWu Scene (*.xw)\0*.xw\0");
		if (!filepath.empty())
		{
			NewScene();

			m_Serializer->Deserialize(filepath);

			m_ActiveScene->SetFilepath(filepath);
		}
	}

	void EditorLayer::SaveScene()
	{
		if (m_ActiveScene->GetFilepath().empty())
		{
			std::string filepath = FileDialogs::SaveFile(L"XuanWu Scene (*.xw)\0*.xw\0");
			if (!filepath.empty())
			{
				m_Serializer->Serialize(filepath);
				m_ActiveScene->SetFilepath(filepath);
			}
		}
		else
		{
			m_Serializer->Serialize(m_ActiveScene->GetFilepath());
		}
	}

	bool EditorLayer::OnWindowResized(WindowResizeEvent& event)
	{
		m_ViewportSize = { event.GetWidth(), event.GetHeight() };

		return false;
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		if(event.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(XW_KEY_LEFT_CONTROL) || Input::IsKeyPressed(XW_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(XW_KEY_LEFT_SHIFT) || Input::IsKeyPressed(XW_KEY_RIGHT_SHIFT);

		switch (event.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();

				break;
			}

			case Key::O:
			{
				if (control)
					OpenScene();

				break;
			}

			case Key::S:
			{
				if (control)
					SaveScene();

				break;
			}
		}
	}

	void EditorLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(XW_BIND_EVENT_FN(EditorLayer::OnWindowResized));
		dispatcher.Dispatch<KeyPressedEvent>(XW_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}
}