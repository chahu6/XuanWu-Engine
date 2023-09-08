#pragma once

#include "OrthographicCamera.h"
#include "XuanWu/Core/Timestep.h"

#include "XuanWu/Events/MouseEvent.h"
#include "XuanWu/Events/ApplicationEvent.h"

namespace XuanWu {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& event);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.f, 0.f, 0.f };
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 20.0f;
		float m_CameraRotation = 0.0f;

		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;
	};
}