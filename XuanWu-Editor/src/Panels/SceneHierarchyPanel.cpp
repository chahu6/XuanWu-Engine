#include "xwpch.h"
#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include "XuanWu/Scene/Components.h"

namespace XuanWu
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin(u8"场景面板");
		{
			m_Context->m_Registry.each([&](auto entityID)
			{
					Entity entity{ entityID, m_Context.get() };
					DrawEntityNode(entity);
			});
		}
		ImGui::End();

		ImGui::ShowDemoWindow();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_None) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		// 必须放到TreeNodeEx函数的下面，不然不行
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			if (ImGui::TreeNodeEx((void*)432435234, flags, tag.c_str()))
			{
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
}