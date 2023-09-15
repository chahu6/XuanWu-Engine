#include "xwpch.h"
#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include "XuanWu/Scene/Components.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui_internal.h>

namespace XuanWu
{
	static void DrawVec3Control(const std::string_view label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.data());
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.data());
		ImGui::NextColumn();

		// �ڶ���
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		// Y
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize)) {
			values.y = resetValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");// 0.1�ٶȣ�0 - 0 ��С���������
		ImGui::PopItemWidth();

		// Z
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize)) {
			values.z = resetValue;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

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
		ImGui::Begin(u8"�������");
		{
			m_Context->m_Registry.each([&](auto entityID)
			{
					Entity entity{ entityID, m_Context.get() };
					DrawEntityNode(entity);
			});

			// ����հ�����ȡ��ѡ��
			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				m_SelectionContext = {};
			}
		}
		ImGui::End();

		ImGui::Begin(u8"�������");
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

		// ����ŵ�TreeNodeEx���������棬��Ȼ����
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
		DrawComponent<TagComponent>("Tag", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen, [](Ref<Scene>& context, Entity selected)
			{
				auto& tag = selected.GetComponent<TagComponent>().Tag;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));

				strcpy(buffer, tag.c_str());
				if (ImGui::InputText(u8"����", buffer, sizeof(buffer)))
				{
					tag = std::string(buffer);
				}
			});

		DrawComponent<TransformComponent>("Tranform", ImGuiTreeNodeFlags_DefaultOpen, [](Ref<Scene>& context, Entity selected)
			{
				auto& tc = selected.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", tc.Translation);
				DrawVec3Control("Rotation", tc.Rotation);
				DrawVec3Control("Scale", tc.Scale, 1.0f);
			});

		DrawComponent<CameraComponent>("Camera", ImGuiTreeNodeFlags_DefaultOpen, [](Ref<Scene>& context, Entity selected)
			{
				auto& cameraComponent = selected.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(uint8_t)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool bIsSelect = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], bIsSelect))
						{
							currentProjectionTypeString = projectionTypeStrings[i];

							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (bIsSelect)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat(u8"�ӳ���", &verticalFov))
					{
						camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));
					}

					float perNear = camera.GetPerspectiveNear();
					if (ImGui::DragFloat("Near", &perNear))
					{
						camera.SetPerspectiveNear(perNear);
					}

					float perFar = camera.GetPerspectiveFar();
					if (ImGui::DragFloat("Far", &perFar))
					{
						camera.SetPerspectiveFar(perFar);
					}
				}
				else
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
					{
						camera.SetOrthographicSize(orthoSize);
					}

					float orthoNear = camera.GetOrthographicNear();
					if (ImGui::DragFloat("Near", &orthoNear))
					{
						camera.SetOrthographicNear(orthoNear);
					}

					float orthoFar = camera.GetOrthographicFar();
					if (ImGui::DragFloat("Far", &orthoFar))
					{
						camera.SetOrthographicFar(orthoFar);
					}

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
				}
			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", ImGuiTreeNodeFlags_DefaultOpen, [](Ref<Scene>& context, Entity selected)
			{
				auto& sprite = selected.GetComponent<SpriteRendererComponent>();
				
				ImGui::DragFloat4(u8"��ɫ", glm::value_ptr(sprite.Color), 0.01f);
			});
	}
}