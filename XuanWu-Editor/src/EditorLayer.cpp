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

		m_CameraController.SetZoomLevel(6.5f);
	}

	void EditorLayer::OnDetach()
	{
		XW_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		XW_PROFILE_FUNCTION();

		if(m_ViewportFocused && m_ViewportHovered)
			m_CameraController.OnUpdate(ts);
		Renderer2D::ResetStats();

		// 绑定帧缓冲
		m_Framebuffer->Bind();

		{
			XW_PROFILE_SCOPE("Renderer Prep");

			RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
			RenderCommand::Clear();
		}
#if 1
		{
			static float rotation = 0.f;
			rotation += ts * 50.f;

			XW_PROFILE_SCOPE("Renderer Draw");
			Renderer2D::BeginScene(m_CameraController.GetCamera());
			Renderer2D::DrawQuad({ -1.0f, -0.5f }, { 0.8f, 0.8f }, { 0.8f, 0.8f, 0.8f , 1.0f });
			Renderer2D::DrawQuad({ -1.0f, 0.5f }, { 0.5f, 0.75f }, m_SquareColor);
			Renderer2D::DrawRotatedQuad({ 0.0f, .0f, 0.1f }, { 1.0f, 1.0f }, glm::radians(rotation), m_Texture, 10.0f);
			Renderer2D::DrawQuad({ .0f, .0f , -0.1f }, { 10.0f, 10.0f }, m_Texture, 1.0f);
			Renderer2D::EndScene();

			Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (float y = -5.f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f, 0.6f };
					Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
				}
			}
			Renderer2D::EndScene();
		}
#endif
		// 解除帧缓冲绑定
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		XW_PROFILE_FUNCTION();

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

			char text[100] = "sdffddsff";
			ImGui::InputText("test", text, IM_ARRAYSIZE(text));
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		{
			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

			//XW_CORE_WARN("Focused: {0}", m_ViewportFocused);
			//XW_CORE_WARN("Hovered: {0}", m_ViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
			{
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
				m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
			}
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2(0, 1), ImVec2(1, 0));
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