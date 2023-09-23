#include "xwpch.h"
#include "glm/gtc/matrix_transform.hpp"

#include "EditorCamera.h"
#include "XuanWu/Core/Input.h"
#include "XuanWu/Core/MouseButtonCode.h"
#include "XuanWu/Core/KeyCode.h"
#include "XuanWu/Core/CursorCode.h"

namespace XuanWu
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		:m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		UpdateView();

		UpdateProjection(); // 临时的
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		if (bIsMoved)
		{
			// 移动
			Move(ts);

			// 视角移动
			MouseMove(ts);
		}
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(XW_BIND_EVENT_FN(EditorCamera::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(XW_BIND_EVENT_FN(EditorCamera::OnMouseButtonReleased));
		dispatcher.Dispatch<MouseScrolledEvent>(XW_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
	}

	void EditorCamera::SetViewportSize(float width, float height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;

		UpdateProjection();
	}

	void EditorCamera::UpdateProjection()
	{
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	}

	void EditorCamera::UpdateVectors()
	{
		m_Forward.x = std::sin(glm::radians(m_Yaw))* std::cos(glm::radians(m_Pitch));
		m_Forward.y = -std::sin(glm::radians(m_Pitch));
		m_Forward.z = -std::cos(glm::radians(m_Yaw)) * std::cos(glm::radians(m_Pitch));

		m_Right = glm::normalize(glm::cross(m_Forward, WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Forward));

		UpdateView();
	}

	void EditorCamera::MoveTo(MovementType type, float speed)
	{
		if (type == MovementType::FORWARD)
			m_Position += m_Forward * speed;
		else if (type == MovementType::BACKWARD)
			m_Position -= m_Forward * speed;
		if (type == MovementType::LEFT)
			m_Position -= m_Right * speed;
		else if (type == MovementType::RIGHT)
			m_Position += m_Right * speed;
		if (type == MovementType::UP)
			m_Position += WorldUp * speed;
		else if (type == MovementType::DOWN)
			m_Position -= WorldUp * speed;

		UpdateView();
	}

	void EditorCamera::MouseMovement(float xoffset, float yoffset, bool bConstrainPitch)
	{
		m_Yaw += xoffset * m_Sensitivity;
		m_Pitch -= yoffset * m_Sensitivity;

		if (bConstrainPitch)
		{
			if (m_Pitch > 89.0f)
				m_Pitch = 89.0f;
			if (m_Pitch < -89.0f)
				m_Pitch = -89.0f;
		}

		UpdateVectors();
	}

	void EditorCamera::Move(Timestep ts)
	{
		if (Input::IsKeyPressed(Key::W))
			MoveTo(FORWARD, m_Speed * ts);
		else if (Input::IsKeyPressed(Key::S))
			MoveTo(BACKWARD, m_Speed * ts);
		if (Input::IsKeyPressed(Key::A))
			MoveTo(LEFT, m_Speed * ts);
		else if (Input::IsKeyPressed(Key::D))
			MoveTo(RIGHT, m_Speed * ts);
		if (Input::IsKeyPressed(Key::E))
			MoveTo(UP, m_Speed * ts);
		else if (Input::IsKeyPressed(Key::Q))
			MoveTo(DOWN, m_Speed * ts);
	}

	bool EditorCamera::MouseMove(Timestep ts)
	{
		float xpos = Input::GetMouseX();
		float ypos = Input::GetMouseY();

		float xoffset = (xpos - lastX) * ts;
		float yoffset = (lastY - ypos) * ts;

		lastX = xpos;
		lastY = ypos;

		MouseMovement(xoffset, yoffset);

		return false;
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_FOV -= e.GetYOffset();
		m_FOV = std::clamp(m_FOV, 1.0f, 75.0f);
		UpdateProjection();

		return false;
	}

	bool EditorCamera::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonRight)
		{
			bIsMoved = true;
			lastX = Input::GetMouseX();
			lastY = Input::GetMouseY();
			
			Input::SetCursorMode(Cursor::CursorDisabled);
		}

		return false;
	}

	bool EditorCamera::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonRight)
		{
			bIsMoved = false;
			Input::SetCursorMode(Cursor::CursorNormal);
		}

		return false;
	}
}