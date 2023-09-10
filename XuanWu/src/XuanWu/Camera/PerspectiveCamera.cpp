#include "xwpch.h"
#include "PerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "XuanWu/Core/Input.h"
#include "XuanWu/Core/KeyCode.h"


namespace XuanWu {

	PerspectiveCamera::PerspectiveCamera(float field, float radio, float n, float f, const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(5.0f), MouseSensitivity(0.05f), Zoom(75.0f)
	{
		Radio = radio;
		zNear = n;
		zFar = f;
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(Position, Position + Front, Up);
		m_ProjectionMatrix = glm::perspective(glm::radians(Zoom), Radio, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(XW_BIND_EVENT_FN(PerspectiveCamera::ProcessMouseScroll));
		dispatcher.Dispatch<MouseMovedEvent>(XW_BIND_EVENT_FN(PerspectiveCamera::ProcessMouseMovement));
	}

	void PerspectiveCamera::OnUpdate(Timestep deltaTime)
	{
		ProcessKeyboard(deltaTime);

		updateCameraVectors();
	}

	void PerspectiveCamera::ProcessKeyboard(Timestep deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (Input::IsKeyPressed(XW_KEY_W))
			Position += Front * velocity;
		else if (Input::IsKeyPressed(XW_KEY_S))
			Position -= Front * velocity;
		if (Input::IsKeyPressed(XW_KEY_A))
			Position -= Right * velocity;
		else if (Input::IsKeyPressed(XW_KEY_D))
			Position += Right * velocity;
		if (Input::IsKeyPressed(XW_KEY_Q))
			Position -= WorldUp * velocity;
		else if (Input::IsKeyPressed(XW_KEY_E))
			Position += WorldUp * velocity;
	}

	bool PerspectiveCamera::ProcessMouseMovement(MouseMovedEvent& event)
	{
		float xpos = event.GetX();
		float ypos = event.GetY();

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		xoffset *= MouseSensitivity;
	    yoffset *= MouseSensitivity;

	    Yaw += xoffset;
	    Pitch += yoffset;

	    // make sure that when pitch is out of bounds, screen doesn't get flipped

	    if (Pitch > 89.0f)
	        Pitch = 89.0f;
	    if (Pitch < -89.0f)
	        Pitch = -89.0f;

		return true;
	}

	bool PerspectiveCamera::ProcessMouseScroll(MouseScrolledEvent& event)
	{
		Zoom -= event.GetYOffset();

		if (Zoom < 45.0f)
			Zoom = 45.0f;
		if (Zoom > 75.0f)
			Zoom = 75.0f;
		return true;
	}

	void PerspectiveCamera::updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));

		RecalculateViewMatrix();
	}
}