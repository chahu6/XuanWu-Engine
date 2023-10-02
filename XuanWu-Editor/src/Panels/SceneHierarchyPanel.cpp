#include "xwpch.h"
#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include "XuanWu/Components/Components.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui_internal.h>

namespace XuanWu
{
	static void DrawVec3Control(const std::string_view label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto font = io.Fonts->Fonts[1];

		ImGui::PushID(label.data());
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.data());
		ImGui::NextColumn();

		// 第二列
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// X
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(font);
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		// Y
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(font);
		if (ImGui::Button("Y", buttonSize)) {
			values.y = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");// 0.1速度，0 - 0 最小最大无限制
		ImGui::PopItemWidth();

		// Z
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(font);
		if (ImGui::Button("Z", buttonSize)) {
			values.z = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		m_AddTexture = Texture2D::Create("Resources/Icons/Add.png");
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::SetSelectedEntity(const Entity& entity)
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin(TXT("场景面板"));
		{
			if (m_Context)
			{
				m_Context->m_Registry.each([&](auto entityID)
				{
					Entity entity{ entityID, m_Context.get() };
					DrawEntityNode(entity);
				});

				// 点击空白区域，取消选中
				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
					m_SelectionContext = {};

				//  ImGuiPopupFlags_NoOpenOverItems 不加这个Flags，右键选项还是创建实体 For BeginPopupContextWindow(): don't return true when hovering items, only when hovering empty space
				if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
				{
					if (ImGui::MenuItem(TXT("创建空实体")))
					{
						m_Context->CreateEntity(TXT("Empty Entity"));
					}
					ImGui::EndPopup();
				}
			}
		}
		ImGui::End();

		ImGui::Begin(TXT("属性面板"));
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
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth; // 将命中框扩展到最右侧边缘，即使没有边框。
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		// 必须放到TreeNodeEx函数的下面，不然不行
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem(TXT("删除实体")))
				entityDeleted = true;
			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (entity == m_SelectionContext)
				m_SelectionContext = {};
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		// ImGuiTreeNodeFlags_Framed: 每个标签都有框了
		// ImGuiTreeNodeFlags_FramePadding: 不知道有什么变化
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<TagComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TagComponent).hash_code(), treeNodeFlags, TXT("Tag")))
			{
				auto& tag = entity.GetComponent<TagComponent>().Tag;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));

				strcpy(buffer, tag.c_str());
				ImGui::Text(TXT("名称:"));
				ImGui::SameLine();
				if (ImGui::InputText(TXT("##名称"), buffer, sizeof(buffer)))
				{
					tag = std::string(buffer);
				}
				ImGui::TreePop();
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button(TXT("添加组件")))
			ImGui::OpenPopup(TXT("AddComponent"));
		if (ImGui::BeginPopup(TXT("AddComponent")))
		{
			if (!m_SelectionContext.HasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem(TXT("Camera")))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
			{
				if (ImGui::MenuItem(TXT("Sprite Renderer")))
				{
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectionContext.HasComponent<Rigidbody2DComponent>())
			{
				if (ImGui::MenuItem(TXT("Rigidbody 2D")))
				{
					m_SelectionContext.AddComponent<Rigidbody2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectionContext.HasComponent<BoxCollider2DComponent>())
			{
				if (ImGui::MenuItem(TXT("Collider 2D")))
				{
					m_SelectionContext.AddComponent<BoxCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>(TXT("Tranform"), entity, treeNodeFlags, [](Ref<Scene>& context, Entity selected)
		{
			auto& tc = selected.GetComponent<TransformComponent>();
			DrawVec3Control(TXT("Translation"), tc.Translation);
			glm::vec3 rotation = glm::degrees(tc.Rotation);
			DrawVec3Control(TXT("Rotation"), rotation);
			tc.Rotation = glm::radians(rotation);
			DrawVec3Control(TXT("Scale"), tc.Scale, 1.0f);
		});

		DrawComponent<CameraComponent>(TXT("Camera"), entity, treeNodeFlags, [](Ref<Scene>& context, Entity selected)
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
				if (ImGui::DragFloat(TXT("视场角"), &verticalFov))
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

		DrawComponent<SpriteRendererComponent>(TXT("Sprite Renderer"), entity, treeNodeFlags, [this](Ref<Scene>& context, Entity selected)
		{
			auto& sprite = selected.GetComponent<SpriteRendererComponent>();

			ImGui::DragFloat4(TXT("颜色"), glm::value_ptr(sprite.Color), 0.01f);

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::Text(TXT("纹理："));
			ImGui::SameLine();
			ImGui::ImageButton((ImTextureID)(sprite.Texture ? sprite.Texture->GetRendererID() : m_AddTexture->GetRendererID()), ImVec2{74, 74}, {0, 1}, {1, 0});
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					if (path != nullptr)
					{
						std::filesystem::path texturePath(path);
						std::string type = texturePath.extension().string();

						bool found = false;
						const char* textureTypes[] = { ".png", ".jpg" };
						for (size_t i = 0; i < sizeof(textureTypes) / sizeof(textureTypes[0]); i++)
						{
							if (type == textureTypes[i])
							{
								found = true;
								break;
							}
						}

						if(found)
							sprite.Texture = Texture2D::Create(texturePath.string());
					}
				}
			}
			ImGui::PopStyleColor();
		});

		DrawComponent<Rigidbody2DComponent>(TXT("Rigidbody 2D"), entity, treeNodeFlags, [this](Ref<Scene>& context, Entity selected)
		{
			auto& rigidbody = selected.GetComponent<Rigidbody2DComponent>();
			
			const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
			const char* currentBodyTypeString = bodyTypeStrings[(int)rigidbody.Type];
			if (ImGui::BeginCombo(TXT("BodyType"), currentBodyTypeString))
			{
				for (int i = 0; i < 3; i++)
				{
					bool bIsSelect = currentBodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], bIsSelect))
					{
						currentBodyTypeString = bodyTypeStrings[i];

						rigidbody.Type = (Rigidbody2DComponent::BodyType)i;
					}
					if (bIsSelect)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Checkbox(TXT("FixedRotation"), &rigidbody.FixedRotation);
		});

		DrawComponent<BoxCollider2DComponent>(TXT("Collider 2D"), entity, treeNodeFlags, [this](Ref<Scene>& context, Entity selected)
		{
			auto& collider = selected.GetComponent<BoxCollider2DComponent>();
			ImGui::DragFloat2(TXT("Offset"), glm::value_ptr(collider.Offset));
			ImGui::DragFloat2(TXT("Size"), glm::value_ptr(collider.Size));
			ImGui::DragFloat(TXT("Density"), &collider.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat(TXT("Friction"), &collider.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat(TXT("Restitution"), &collider.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat(TXT("Restitution Threshold"), &collider.RestitutionThreshold, 0.01f, 0.0f);
		});
	}
}