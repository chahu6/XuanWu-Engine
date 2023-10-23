#pragma once
#include "XuanWu/Render/Texture.h"
#include "XuanWu/Events/Event.h"
#include "XuanWu/Events/MouseEvent.h"

namespace XuanWu
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

		void OnEvent(Event& event);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);

	private:
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_DirectoryIcon;

		float m_Padding = 16.0f;
		float m_ThumbnailSize = 128.0f;

	public:
	};
}