#include "xwpch.h"
#include "Application.h"

#include "Input.h"
#include "KeyCode.h"
#include "XuanWu/Core/Platform.h"
#include "XuanWu/Render/Renderer.h"
#include "XuanWu/Scripting/ScriptEngine.h"

namespace XuanWu {

//#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		:m_Specification(specification)
	{
		XW_PROFILE_FUNCTION();

		XW_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// 设置工作目录
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(specification.Name)));
		m_Window->SetEventCallback(XW_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		ScriptEngine::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		XW_PROFILE_FUNCTION();

		ScriptEngine::Shutdown(); 
		Renderer::Shutdown(); // 他应该最后关
	}

	void Application::OnEvent(Event& e)
	{
		XW_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(XW_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(XW_BIND_EVENT_FN(Application::OnWindowResize));
		if (XuanWu::Input::IsKeyPressed(Key::Escape))
			m_Running = false;

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			if (e.Handled)
				break;
			(*--it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		XW_PROFILE_FUNCTION();

		while (m_Running)
		{
			XW_PROFILE_SCOPE("RunLoop");

			float currentTime = Platform::GetTime();
			Timestep timestep = currentTime - m_LastFrameTime;
			m_LastFrameTime = currentTime;

			if (!m_Minimized)
			{
				{
					XW_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					XW_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		XW_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		XW_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		Close();
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		XW_PROFILE_FUNCTION();

		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());

		return false;
	}
}