#include "xwpch.h"
#include "EditorLayer.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include "XuanWu/Render/Renderer2D.h"

namespace XuanWu
{
	EditorLayer::EditorLayer()
		:Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, false)
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

		m_CameraController.SetZoomLevel(4.5f);

		m_ActiveScene = CreateRef<Scene>();

		m_SquareEntity = m_ActiveScene->CreateEntity(u8"方块");
		m_SquareEntity.AddComponent<SpriteRendererComponent>();

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Second Camera");
		m_SecondCamera.AddComponent<CameraComponent>();

		m_SecondCamera.GetComponent<CameraComponent>().Primary = false;
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
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		//没被选中就不能动
		if(m_ViewportFocused && m_ViewportHovered)
			m_CameraController.OnUpdate(ts);

		// 重置Stats统计的数据
		Renderer2D::ResetStats();

		// 绑定帧缓冲
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
		RenderCommand::Clear();

		m_ActiveScene->OnUpdate(ts);

		// 解除帧缓冲绑定
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		XW_PROFILE_FUNCTION();

		// 菜单栏
		if (ImGui::BeginMainMenuBar())
		{
			// 添加一个菜单
			if (ImGui::BeginMenu("Options"))
			{
				// 添加菜单项
				if (ImGui::MenuItem("Exit"))
				{
					Application::Get().Close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::Begin("Settings");
		{
			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats: ");
			ImGui::Text(u8"绘制次数：%d", stats.DrawCalls);
			ImGui::Text(u8"四边形数量：%d", stats.QuadCount);
			ImGui::Text(u8"顶点数量：%d", stats.GetTotalVertexCount());
			ImGui::Text(u8"索引数量：%d", stats.GetTotalIndexCount());
			
			if (m_SquareEntity)
			{
				ImGui::Separator();
				auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
				auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
				ImGui::Text(tag.c_str());
				ImGui::ColorEdit4(u8"颜色编辑器", &squareColor.x /* glm::value_ptr(squareColor) */);
				ImGui::Separator();
			}
			
			ImGui::Text(u8"Camera");
			ImGui::DragFloat3("Camera Transform",
				glm::value_ptr(m_SecondCamera.GetComponent<TransformComponent>().Transform[3]));
			if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
			{
				m_CameraEntity.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
				m_SecondCamera.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
			}
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		{
			m_ViewportFocused = ImGui::IsWindowFocused();//  聚焦为true
			m_ViewportHovered = ImGui::IsWindowHovered();// 悬停为true
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
		ImGui::Begin("DepthTexture");
		{
			uint32_t textureID = m_Framebuffer->GetDepthAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(XW_BIND_EVENT_FN(EditorLayer::OnWindowResized));

		m_CameraController.OnEvent(event);
	}

	bool EditorLayer::OnWindowResized(WindowResizeEvent& event)
	{
		m_ViewportSize = { event.GetWidth(), event.GetHeight() };

		return false;
	}
}