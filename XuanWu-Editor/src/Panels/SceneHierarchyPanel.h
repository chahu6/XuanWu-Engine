#pragma once

#include "XuanWu/Scene/Scene.h"
#include "XuanWu/Scene/Entity.h"

namespace XuanWu
{
	class SceneHierarchyPanel
	{
	public:
		using DrawComponentFunc = void(*)(Ref<Scene>& context, Entity selected);

		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		template<typename T>
		void DrawComponent(std::string_view name, char flags, DrawComponentFunc func)
		{
			if (m_SelectionContext.HasComponent<T>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.data()))
				{
					func(m_Context, m_SelectionContext);
					ImGui::TreePop();
					ImGui::Separator();
				}
			}
		}

	private:
		Ref<Scene> m_Context;

		Entity m_SelectionContext;
	};
}