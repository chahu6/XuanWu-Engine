#pragma once

#include "XuanWu/Core.h"
#include "XuanWu/Events/Event.h"
#include "XuanWu/Core/Timestep.h"

namespace XuanWu {

	class XUANWU_API Layer
	{
	public:
		Layer(const std::string& debugname = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}