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
		void SetSelectedEntity(const Entity& entity);

		void OnImGuiRender();

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		template<typename T>
		void DrawComponent(std::string_view name, Entity entity, char flags, DrawComponentFunc func)
		{
			if (m_SelectionContext.HasComponent<T>())
			{
				ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
				bool bIsOpen = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.data());

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup(TXT("ComponentSettings"));
				}
				ImGui::PopStyleVar();

				bool removeComponent = false;
				if (ImGui::BeginPopup(TXT("ComponentSettings")))
				{
					if (ImGui::MenuItem(TXT("ÒÆ³ý×é¼þ")))
						removeComponent = true;

					ImGui::EndPopup();
				}

				if (bIsOpen)
				{
					func(m_Context, m_SelectionContext);

					ImGui::TreePop();
				}
				ImGui::Separator();

				if (removeComponent)
				{
					entity.RemoveComponent<T>();
				}
			}
		}

	private:
		Ref<Scene> m_Context;

		Entity m_SelectionContext;

	public:
		inline Entity GetSelectedEntity() { return m_SelectionContext; }
	};
}