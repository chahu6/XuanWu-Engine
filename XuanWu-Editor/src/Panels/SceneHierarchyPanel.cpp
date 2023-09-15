#include "xwpch.h"
#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include "XuanWu/Scene/Components.h"
#include <glm/gtc/type_ptr.hpp>

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

			// 点击空白区域，取消选中
			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};
		}
		ImGui::End();

		ImGui::Begin(u8"属性面板");
		{
			if (m_SelectionContext)
			{
				DrawComponents(m_SelectionContext);
			}
		}
		ImGui::End();
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
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));

			strcpy(buffer, tag.c_str());
			if (ImGui::InputText(u8"名称", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Tranform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
				// 展开树节点
				ImGui::TreePop();
			}
		}
	}
}