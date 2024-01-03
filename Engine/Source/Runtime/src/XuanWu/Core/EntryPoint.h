#pragma once

#ifdef XW_PLATFORM_WINDOWS 

extern XuanWu::Application* XuanWu::CreateApplication(XuanWu::ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	XuanWu::Log::Init();

	XW_PROFILE_BEGIN_SESSION("Startup", "XuanWuProfile-Startup.json");
	auto app = XuanWu::CreateApplication({ argc, argv });
	XW_PROFILE_END_SESSION();

	XW_PROFILE_BEGIN_SESSION("Runtime", "XuanWuProfile-Runtime.json");
	app->Run();
	XW_PROFILE_END_SESSION();

	XW_PROFILE_BEGIN_SESSION("Shutdown", "XuanWuProfile-Shutdown.json");
	delete app;
	XW_PROFILE_END_SESSION();
}

#endif // XW_PLATFORM_WINDOWS 