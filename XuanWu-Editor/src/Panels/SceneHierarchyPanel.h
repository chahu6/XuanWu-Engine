#pragma once

#include "XuanWu/Scene/Scene.h"
#include "XuanWu/Scene/Entity.h"
#include "XuanWu/Render/Texture.h"

namespace XuanWu
{
	class SceneHierarchyPanel
	{
	public:
		//using DrawComponentFunc = void(*)(Ref<Scene>& context, Entity selected); // ���ָ���޷�����[this](Ref<Scene>&, Entity){}������˸�this��lambda���ʽ�����Ի���
		using DrawComponentFunc = std::function<void(Ref<Scene>&, Entity)>;

		SceneHierarchyPanel();
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
				float lineHeight = ImGui::GetCurrentContext()->Font->FontSize + ImGui::GetCurrentContext()->Style.FramePadding.y * 2.0f;
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup(TXT("ComponentSettings"));
				}
				ImGui::PopStyleVar();

				bool removeComponent = false;
				if (ImGui::BeginPopup(TXT("ComponentSettings")))
				{
					if (ImGui::MenuItem(TXT("�Ƴ����")))
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

		template<typename Component>
		void DisplayAddComponentEntity(const std::string_view name)
		{
			if (!m_SelectionContext.HasComponent<Component>())
			{
				if (ImGui::MenuItem(name.data()))
				{
					m_SelectionContext.AddComponent<Component>();
					ImGui::CloseCurrentPopup();
				}
			}
		}
	private:
		Ref<Scene> m_Context;

		Entity m_SelectionContext;

		Ref<Texture2D> m_AddTexture;

	public:
		inline Entity GetSelectedEntity() { return m_SelectionContext; }
	};
}