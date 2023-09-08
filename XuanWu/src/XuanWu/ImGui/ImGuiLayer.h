#pragma once

#include "XuanWu/Core/Layer.h"
#include "XuanWu/Events/KeyEvent.h"
#include "XuanWu/Events/MouseEvent.h"
#include "XuanWu/Events/ApplicationEvent.h"

namespace XuanWu {

	class XUANWU_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = .0f;
	};
}