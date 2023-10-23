#pragma once

#include "XuanWu/Core/Timestep.h"

#include "XuanWu/Events/ApplicationEvent.h"
#include "PerspectiveCamera.h"

namespace XuanWu {

	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController();

		void OnUpdate(Timestep ts);
		void OnEvent(Event& event);

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }
	private:

	private:

		PerspectiveCamera m_Camera;
	};
}