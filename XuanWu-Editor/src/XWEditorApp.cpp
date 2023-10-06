#include <XuanWu.h>
#include <XuanWu/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace XuanWu
{
	class XuanWuEditor : public Application
	{
	public:
		XuanWuEditor(const ApplicationSpecification& spec)
			:Application(spec)
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}

		~XuanWuEditor()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "XuanWu-Editor";
		spec.CommandLineArgs = args;

		return new XuanWuEditor(spec);
	}
}
