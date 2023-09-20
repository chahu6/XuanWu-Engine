#pragma once

#include "Camera.h"
#include "XuanWu/Core/Timestep.h"
#include "XuanWu/Events/Event.h"
#include "XuanWu/Events/MouseEvent.h"

namespace XuanWu
{
	// 世界向上向量
	constexpr glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };

	class EditorCamera : public Camera
	{
		enum MovementType : uint8_t
		{
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT,
			UP,
			DOWN
		};
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio = 1.778f, float nearClip = 0.1f, float farClip = 1000.0f);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void SetViewportSize(float width, float height);
	private:
		void UpdateProjection();
		void UpdateVectors();
		void UpdateView();

		void Move(Timestep ts);
		void MoveTo(MovementType type, float speed);

		bool MouseMove(Timestep ts);
		void MouseMovement(float xoffset, float yoffset, bool bConstrainPitch = true);

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
	private:
		float m_FOV = 45.0f;
		float m_AspectRatio = 1.778f;
		float m_NearClip = 0.1f, m_FarClip = 1000.0f;

		float m_Speed = 8.0f;

		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::vec3 m_Position = { 0.0f,0.0f, 5.0f };
		glm::vec3 m_FocalPoint = glm::vec3(0.0f);

		glm::vec3 m_Forward = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Right = { 1.0f, 0.0f, 0.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_Sensitivity = 6.0f;

		float m_ViewportWidth = 1600.0f, m_ViewportHeight = 900.0f;
	private:
		float lastX = 0.0f;
		float lastY = 0.0f;
		bool bIsMoved = false;
	public:
		inline glm::mat4 GetViewMatrix() { return m_ViewMatrix; };
		inline glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }
	};
}