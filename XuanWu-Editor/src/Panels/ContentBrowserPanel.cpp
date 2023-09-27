#include "xwpch.h"
#include "ContentBrowserPanel.h"
#include "XuanWu/Core/Input.h"
#include "XuanWu/Core/KeyCode.h"

#include <imgui/imgui.h>

namespace XuanWu
{
	static const std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		:m_CurrentDirectory(s_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin(TXT("内容浏览器"));
		{
			if (m_CurrentDirectory != s_AssetPath)
			{
				if (ImGui::Button("<-"))
				{
					m_CurrentDirectory = m_CurrentDirectory.parent_path();
				}
			}

			float cellSize = m_ThumbnailSize + m_Padding;

			float panelWidth = ImGui::GetContentRegionAvail().x;
			int columnCount = (int)(panelWidth / cellSize);
			if (columnCount < 1)
			{
				columnCount = 1;
			}

			ImGui::Columns(columnCount, 0, false);

			std::filesystem::directory_iterator iter(m_CurrentDirectory);

			for (const auto& directoryEntry : iter)
			{
				// 1、得到子文件夹或文件path类
				const auto& path = directoryEntry.path();
				// 2、获取子文件夹或文件的名称
				const std::string& filenameString = path.filename().string();

				//Push唯一ID，用于拖拽
				ImGui::PushID(filenameString.c_str());

				Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
				
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { m_ThumbnailSize, m_ThumbnailSize }, { 0, 1 }, { 1, 0 });
				if (ImGui::BeginDragDropSource())
				{
					if (directoryEntry.is_regular_file())
					{
						// 设置数据源
						const wchar_t* itemPath = path.c_str();
						ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
					}
					ImGui::EndDragDropSource();
				}
				ImGui::PopStyleColor();

				// 必须悬停和双击
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (directoryEntry.is_directory())
					{
						m_CurrentDirectory /= filenameString;
					}
				}
				ImGui::TextWrapped(filenameString.c_str());
				ImGui::NextColumn();

				ImGui::PopID();
			}
			// 设置滑动条
			ImGui::Columns(1);

			ImGui::SliderFloat(TXT("图标尺寸"), &m_ThumbnailSize, 16, 512);
			//ImGui::SliderFloat("Padding", &padding, 0, 32);
		}
		ImGui::End();
	}

	bool ContentBrowserPanel::OnMouseScrolled(MouseScrolledEvent& event)
	{
		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		
		if (control)
		{
			m_ThumbnailSize += event.GetYOffset();
			m_ThumbnailSize = std::clamp(m_ThumbnailSize, 64.0f, 512.0f);
		}

		return true;
	}

	void ContentBrowserPanel::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(XW_BIND_EVENT_FN(ContentBrowserPanel::OnMouseScrolled));
	}
}
