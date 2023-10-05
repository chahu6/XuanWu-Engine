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

		/*m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_SpriteTexture = Texture2D::Create("assets/games/RPGpack_sheet_2X.png");*/

		m_IconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
		m_IconStop = Texture2D::Create("Resources/Icons/StopButton.png");
		m_IconSimulate = Texture2D::Create("Resources/Icons/SimulateButton.png");

		FramebufferSpecification spec;
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);

		m_EditorScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(45.0f, 1.778f);
		m_ActiveScene = m_EditorScene;
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
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		// 重置Stats统计的数据
		Renderer2D::ResetStats();

		// 绑定帧缓冲
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
		RenderCommand::Clear();

		// 用-1填冲帧缓冲的第二个颜色缓冲区
		m_Framebuffer->ClearAttachment(1, -1);

		switch (m_SceneState)
		{
			case XuanWu::EditorLayer::SceneState::Edit:
			{
				m_EditorCamera.OnUpdate(ts);
				m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
				break;
			}
			case XuanWu::EditorLayer::SceneState::Play:
			{
				m_ActiveScene->OnUpdateRuntime(ts);
				break;
			}
			case XuanWu::EditorLayer::SceneState::Simulation:
			{
				m_EditorCamera.OnUpdate(ts);
				m_ActiveScene->OnUpdateSimulation(ts, m_EditorCamera);
				break;
			}
			default:
				XW_CORE_ASSERT(false, "没有这种状态");
				break;
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		// 反转Y轴，使得左下角为(0, 0)点
		my = viewportSize.y - my;

		int mouseX = static_cast<int>(mx);
		int mouseY = static_cast<int>(my);

		if (mouseX >= 0 && mouseY >= 0 && mouseX <= static_cast<int>(viewportSize.x) && mouseY <= static_cast<int>(viewportSize.y))
		{
			// @TDOD 读取第几个帧缓冲的位置的值
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			//XW_CORE_WARN("{0}", pixelData);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity{ (entt::entity)pixelData, m_ActiveScene.get() };
		}

		// 调式画碰撞盒框
		OnOverlayRender();

		// 解除帧缓冲绑定
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		XW_PROFILE_FUNCTION();

		//场景面板渲染
		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		// 菜单栏
		if (ImGui::BeginMainMenuBar())
		{
			// 添加一个菜单
			if (ImGui::BeginMenu(TXT("File")))
			{
				// 添加菜单项
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

		// 设置
		ImGui::Begin(TXT("Settings"));
		{
			ImGui::Checkbox(TXT("Show Physics Colliders"), &m_ShowPhysicsColliders);
		}
		ImGui::End();

		// 统计数据
		ImGui::Begin(TXT("Stats"));
		{
			auto stats = Renderer2D::GetStats();
			ImGui::Text(TXT("Renderer2D Stats: "));
			ImGui::Text(TXT("绘制次数：%d"), stats.DrawCalls);
			ImGui::Text(TXT("四边形数量：%d"), stats.QuadCount);
			ImGui::Text(TXT("顶点数量：%d"), stats.GetTotalVertexCount());
			ImGui::Text(TXT("索引数量：%d"), stats.GetTotalIndexCount());
		}
		ImGui::End();
		
		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(TXT("Viewport"));
		{
			// 也可以，但是有更好的
			ImVec2 windowSize = ImGui::GetWindowSize();
			ImVec2 minBound = ImGui::GetWindowPos();
			// 必须放在这，下面的代码有可能影响了他，反正放下面数值就不对了
			ImVec2 viewportOffset = ImGui::GetCursorPos();

			minBound.x += viewportOffset.x;
			minBound.y += viewportOffset.y;
			windowSize.y -= viewportOffset.y;

			ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
			m_ViewportBounds[0] = { minBound.x, minBound.y };
			m_ViewportBounds[1] = { maxBound.x, maxBound.y };

			// GetWindowContentRegionMin() 是以自己窗口内容的左上角为起点(0, 0) 反正我不会弄，后面再说吧
		/*	ImVec2 viewportMinRegion = ImGui::GetWindowContentRegionMin();
			ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			m_ViewportBound[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
			m_ViewportBound[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
			*/

			m_ViewportFocused = ImGui::IsWindowFocused();//  聚焦为true
			m_ViewportHovered = ImGui::IsWindowHovered();// 悬停为true
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered); // 聚焦有点毛病，

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
			{
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			}

			// 渲染的哪个帧缓冲区
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));

			// Gizmos
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				//float windowWidth = ImGui::GetWindowWidth(); // 获取 当前的 窗口宽度
				//float windowHeight = ImGui::GetWindowHeight();
				//ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
				ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

				// Editor 时
				const glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();
				const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();

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

			// 接收拖拽的负载
			if (ImGui::BeginDragDropTarget())
			{
				if (m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulation)
					OnSceneStop();

				// 因为接收内容可能为空，需要if判断 。 CONTENT_BROWSER_ITEM：拖动携带的内容
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path directory(path);
					if(path != nullptr && directory.extension().string() == ".scene")
						OpenScene(directory);
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();
	}

	void EditorLayer::NewScene()
	{
		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_Serializer->SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile(L"XuanWu Scene (*.scene)\0*.scene\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		NewScene();

		m_Serializer->Deserialize(path.string());

		m_ActiveScene->SetFilepath(path.string());
	}

	void EditorLayer::SaveScene()
	{
		if (m_ActiveScene->GetFilepath().empty())
		{
			std::string filepath = FileDialogs::SaveFile(L"XuanWu Scene (*.scene)\0*.scene\0");
			filepath += ".scene";
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

	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit) return;

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

		if (selectedEntity)
			m_EditorScene->DuplicateEntity(selectedEntity);
	}

	void EditorLayer::UI_Toolbar()
	{
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 2 });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));// item之间的间隔
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0, 0, 0, 0 });

		auto& colors = ImGui::GetStyle().Colors;
		// 按钮针对hover、click有不同效果
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{
			// Play
			{
				Ref<Texture2D> icon = m_SceneState != SceneState::Play ? m_IconPlay : m_IconStop;
				float size = ImGui::GetWindowHeight() - 18.0f;

				ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f - (size * 0.5f));
				if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2{ size, size }, { 0, 1 }, { 1, 0 })) // 64.0f
				{
					switch (m_SceneState)
					{
					case SceneState::Edit:
						OnScenePlay();
						break;
					case SceneState::Simulation:
					case SceneState::Play:
						OnSceneStop();
						break;
					default:
						XW_CORE_ASSERT(false, "未知状态");
						break;
					}
				}
			}
			ImGui::SameLine();
			// Simulation
			{
				Ref<Texture2D> icon = m_SceneState != SceneState::Simulation ? m_IconSimulate : m_IconStop;
				float size = ImGui::GetWindowHeight() - 18.0f;
				if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2{ size, size }, { 0, 1 }, { 1, 0 })) // 64.0f
				{
					switch (m_SceneState)
					{
					case SceneState::Edit:
						OnSceneSimulate();
						break;
					case SceneState::Play:
					case SceneState::Simulation:
						OnSceneStop();
						break;
					default:
						XW_CORE_ASSERT(false, "未知状态");
						break;
					}
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
	}

	void EditorLayer::OnScenePlay()
	{
		if (!m_EditorScene)
		{
			XW_CORE_ASSERT(false, "EditorScene is nullptr");
			return;
		}
		m_SceneState = SceneState::Play;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneSimulate()
	{
		XW_CORE_ASSERT(m_EditorScene, "EditorScene is nullptr");

		m_SceneState = SceneState::Simulation;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		XW_CORE_ASSERT(m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulation, "未知状态");

		if (m_SceneState == SceneState::Play)
			m_ActiveScene->OnRuntimeStop();
		else if (m_SceneState == SceneState::Simulation)
			m_ActiveScene->OnSimulationStop();
			
		m_SceneState = SceneState::Edit;

		m_ActiveScene = m_EditorScene;

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity entity = m_ActiveScene->GetPrimaryCameraEntity();
			if (!entity)
				return;

			Renderer2D::BeginScene(entity.GetComponent<CameraComponent>().Camera, entity.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{
			// Draw Box Collider
			{
				auto& view = m_ActiveScene->GetAllComponentView<TransformComponent, BoxCollider2DComponent>();
				for (auto e : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(e);
					glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);
					glm::mat4 rotation = glm::toMat4(glm::quat(tc.Rotation));

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* rotation
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawRect(transform, { 0.0f, 1.0f, 0.0f, 1.0f });
				}
			}

			// Draw Circle Collider
			{
				auto& view = m_ActiveScene->GetAllComponentView<TransformComponent, CircleCollider2DComponent>();
				for (auto e : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(e);
					glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);
					glm::mat4 rotation = glm::toMat4(glm::quat(tc.Rotation));

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* rotation
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawCircle(transform, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.02f);
				}
			}
		}

		Renderer2D::EndScene();
	}

	bool EditorLayer::OnWindowResized(WindowResizeEvent& event)
	{
		m_ViewportSize = { event.GetWidth(), event.GetHeight() };

		return false;
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		if(event.IsRepeat())
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
			case Key::D:
			{
				if (control)
					OnDuplicateEntity();

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
		// 内容浏览器Ctrl + 鼠标滚动 调整图片大小
		m_ContentBrowserPanel.OnEvent(event); // @TODO 测试，因为Viewport失去焦点，所以捕获了鼠标和键盘事件，所以他也不行了，只能Viewport获取焦点，他才能执行到，以后再修复吧

		m_EditorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(XW_BIND_EVENT_FN(EditorLayer::OnWindowResized));
		dispatcher.Dispatch<KeyPressedEvent>(XW_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(XW_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}
}