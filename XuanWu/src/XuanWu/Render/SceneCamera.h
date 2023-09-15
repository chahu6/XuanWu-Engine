#pragma once

#include "Camera.h"

namespace XuanWu
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType : uint8_t
		{
			Perspective = 0,
			Orthographic = 1
		};

	public:
		SceneCamera();
		~SceneCamera() = default;

		void SetViewportSize(uint32_t width, uint32_t height);
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float fov, float nearClip, float farClip);

		// 正交
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		void SetOrthographicNear(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		void SetOrthographicFar(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

		// 透视
		void SetPerspectiveVerticalFOV(float fov) { m_PerspectiveVerticalFOV = fov; RecalculateProjection(); }
		void SetPerspectiveNear(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		void SetPerspectiveFar(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		// 类型
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		// 正交
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		// 透视
		float m_PerspectiveVerticalFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		// 宽高比
		float m_AspectRatio = 0.0f;

	public:
		inline ProjectionType GetProjectionType() { return m_ProjectionType; }
		inline float GetOrthographicSize() { return m_OrthographicSize; }
		inline float GetOrthographicNear() { return m_OrthographicNear; }
		inline float GetOrthographicFar() { return m_OrthographicFar; }
		inline float GetPerspectiveVerticalFOV() { return m_PerspectiveVerticalFOV; }
		inline float GetPerspectiveNear() { return m_PerspectiveNear; }
		inline float GetPerspectiveFar() { return m_PerspectiveFar; }
	};
}