#include "xwpch.h"
#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "XuanWu/Render/Renderer2D.h"

#include "XuanWu/Utils/PlatformUtils.h"
#include <ImGuizmo.h>
#include "XuanWu/Math/Math.h"

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
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);

		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(45.0f, 1.778f);

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

		m_EditorCamera.OnUpdate(ts);

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && 
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		// ����Statsͳ�Ƶ�����
		Renderer2D::ResetStats();

		// ��֡����
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
		RenderCommand::Clear();

		// ��-1���֡����ĵڶ�����ɫ������
		m_Framebuffer->ClearAttachment(1, -1);

		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBound[0].x;
		my -= m_ViewportBound[0].y;

		glm::vec2 viewportSize = m_ViewportBound[1] - m_ViewportBound[0];

		// ��תY�ᣬʹ�����½�Ϊ(0, 0)��
		my = viewportSize.y - my;

		int mouseX = static_cast<int>(mx);
		int mouseY = static_cast<int>(my);

		if (mouseX >= 0 && mouseY >= 0 && mouseX <= static_cast<int>(viewportSize.x) && mouseY <= static_cast<int>(viewportSize.y))
		{
			// @TDOD ��ȡ�ڼ���֡�����λ�õ�ֵ
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity{ (entt::entity)pixelData, m_ActiveScene.get() };
		}

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
			// Ҳ���ԣ������и��õ�
			ImVec2 windowSize = ImGui::GetWindowSize();
			ImVec2 minBound = ImGui::GetWindowPos();
			// ��������⣬����Ĵ����п���Ӱ��������������������ֵ�Ͳ�����
			ImVec2 viewportOffset = ImGui::GetCursorPos();

			minBound.x += viewportOffset.x;
			minBound.y += viewportOffset.y;
			windowSize.y -= viewportOffset.y;

			ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
			m_ViewportBound[0] = { minBound.x, minBound.y };
			m_ViewportBound[1] = { maxBound.x, maxBound.y };

			// GetWindowContentRegionMin() �����Լ��������ݵ����Ͻ�Ϊ���(0, 0) �����Ҳ���Ū��������˵��
		/*	ImVec2 viewportMinRegion = ImGui::GetWindowContentRegionMin();
			ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			m_ViewportBound[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
			m_ViewportBound[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
			*/

			m_ViewportFocused = ImGui::IsWindowFocused();//  �۽�Ϊtrue
			m_ViewportHovered = ImGui::IsWindowHovered();// ��ͣΪtrue
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered); // �۽��е�ë����

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
			{
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			}

			// ��Ⱦ���ĸ�֡������
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));

			// Gizmos
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				float windowWidth = ImGui::GetWindowWidth(); // ��ȡ ��ǰ�� ���ڿ��
				float windowHeight = ImGui::GetWindowHeight();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

				// Editor ʱ
				const glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();
				const glm::mat4 cameraProjection = m_EditorCamera.GetProjection();

				// Runtime ʱ
				/*auto& cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
				const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
				const glm::mat4& cameraProjection = camera.GetProjection();
				glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/

				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetTransform();

				bool snap = Input::IsKeyPressed(Key::LeftControl);
				float snapValue = 0.05f;
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 Translation, Rotation, Scale;
					Math::DecomponseTrasnform(transform, Translation, Rotation, Scale);

					tc.Translation = Translation;
					tc.Rotation = Rotation;
					tc.Scale = Scale;
				}
			}

		}
		ImGui::End();


		/*ImGui::Begin(TXT("DepthTexture"));
		{
			uint32_t textureID = m_Framebuffer->GetDepthAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();*/
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

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

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

			default:
			{
				if (!m_EditorCamera.IsMoved())
				{
					switch (event.GetKeyCode())
					{
						case Key::W:
							m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
							break;
						case Key::E:
							m_GizmoType = ImGuizmo::OPERATION::ROTATE;
							break;
						case Key::R:
							m_GizmoType = ImGuizmo::OPERATION::SCALE;
							break;
					}
				}
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		if (event.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver())
			{
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
			}
		}

		return false;
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_EditorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(XW_BIND_EVENT_FN(EditorLayer::OnWindowResized));
		dispatcher.Dispatch<KeyPressedEvent>(XW_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(XW_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}
}