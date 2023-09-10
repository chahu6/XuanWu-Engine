#include <XuanWu.h>
#include <XuanWu/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace XuanWu
{
	class XuanWuEditor : public Application
	{
	public:
		XuanWuEditor()
			:Application("XuanWu-Editor")
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}

		~XuanWuEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new XuanWuEditor();
	}
}
