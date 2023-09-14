#pragma once

#include "XuanWu/Scene/Scene.h"
#include "XuanWu/Scene/Entity.h"

namespace XuanWu
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context;

		Entity m_SelectionContext;
	};
}