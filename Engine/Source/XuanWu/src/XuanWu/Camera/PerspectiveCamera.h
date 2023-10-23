#pragma once

#include <glm/glm.hpp>
#include "XuanWu/Events/Event.h"
#include "XuanWu/Core/Timestep.h"
#include "XuanWu/Events/Event.h"
#include "XuanWu/Events/MouseEvent.h"

namespace XuanWu {

	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float field, float radio, float near, float far, const glm::vec3& position = glm::vec3(0.f), const glm::vec3& up = glm::vec3(0.f, 1.0f, 0.f), float yaw = -90.0f, float pitch = 0.0f);

		inline const glm::vec3& GetPosition() const { return Position; }
		void SetPosition(const glm::vec3& position) { Position = position; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		void OnEvent(Event& event);
		void OnUpdate(Timestep deltaTime);
	private:
		void RecalculateViewMatrix();
		void updateCameraVectors();

		void ProcessKeyboard(Timestep deltaTime);
		bool ProcessMouseMovement(MouseMovedEvent& event);
		bool ProcessMouseScroll(MouseScrolledEvent& event);
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 Position;
		glm::vec3 WorldUp;
		glm::vec3 Right;
		glm::vec3 Up;
		glm::vec3 Front;

		float Zoom;
		float Yaw;
		float Pitch;
		float MovementSpeed;
		float MouseSensitivity;
		float Radio;
		float zNear;
		float zFar;


		float lastX = 1280 / 2;
		float lastY = 720 / 2;
	};
}