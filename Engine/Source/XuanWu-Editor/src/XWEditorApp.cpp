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
		/*Color color{ 3.0, 2.0f, 4.5f };

		auto myClass = Color::StaticClass();
		auto name = myClass->GetName();

		auto field = myClass->GetField("r");
		auto sdt = *(float*)field->GetValue(&color);
		float sss = 6546.456f;
		field->SetValue(&color, &sss);*/

		ApplicationSpecification spec;
		spec.Name = "XuanWu-Editor";
		spec.CommandLineArgs = args;


		return new XuanWuEditor(spec);
	}
}
