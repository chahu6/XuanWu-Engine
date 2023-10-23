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
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = .0f;
	};
}