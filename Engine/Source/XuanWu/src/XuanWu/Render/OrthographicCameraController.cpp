#include "xwpch.h"
#include "OrthographicCameraController.h"

#include "XuanWu/Core/Input.h"
#include "XuanWu/Core/KeyCode.h"

namespace XuanWu {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_AspectRatio(aspectRatio), m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), m_Rotation(rotation),
		m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel })
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		XW_PROFILE_FUNCTION();

		if (XuanWu::Input::IsKeyPressed(Key::W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		else if (XuanWu::Input::IsKeyPressed(Key::S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		if (XuanWu::Input::IsKeyPressed(Key::A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		else if (XuanWu::Input::IsKeyPressed(Key::D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		XW_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(XW_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(XW_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		if (height == 0.0f) return;

		m_AspectRatio = width / height;
		CalculateView();
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		XW_PROFILE_FUNCTION();

		m_ZoomLevel -= event.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.15f);
		m_ZoomLevel = std::min(m_ZoomLevel, 10.0f);
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		XW_PROFILE_FUNCTION();

		OnResize((float)event.GetWidth(), (float)event.GetHeight());
		return false;
	}
}