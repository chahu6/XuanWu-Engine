#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "XuanWu/Events/Event.h"
#include "XuanWu/Events/ApplicationEvent.h"

#include "XuanWu/ImGui/ImGuiLayer.h"
#include "XuanWu/Core/Timestep.h"


namespace XuanWu {

	class Application
	{
	public:
		Application(const std::string& name = "XuanWu App");

		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void Close();
		
		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}


