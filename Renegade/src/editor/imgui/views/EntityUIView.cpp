#ifdef __EDITOR__

#include "editor/imgui/views/EntityUIView.h"

#include <imgui/imgui_helpers.h>

#include "editor/imgui/EditorSelectable.h"
#include "core/Engine.h"
#include "editor/imgui/ImGuiDefines.h"
#include "utils/string_extensions.h"
#include "editor/imgui/ImGuiWindow.h"
#include "gameplay/systems/TransformSystem.h"
#include "editor/imgui/views/TransformComponentUIView.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			EntityUIView::EntityUIView(ImGuiWindow& a_Window, gameplay::EntityID& a_EntityID) : EditorSelectable(a_Window)
			{
				m_EntityID = &a_EntityID;
			}

			std::string EntityUIView::GetIcon() const
			{
				return ICON_FA_CUBE;
			}

			void EntityUIView::Render(bool& clicked, bool selected)
			{
				ImDrawList* draw_list = ImGui::GetWindowDrawList();

				ImVec2 pos = ImGui::GetCursorScreenPos();

				ImVec2 select_min = pos;
				ImVec2 select_max = ImVec2(select_min.x + ImGui::GetContentRegionAvail().x, select_min.y + m_Window.FontSize());

				bool is_hovered = ImGui::IsMouseHoveringRect(select_min, select_max);
				if (selected || is_hovered)
				{
					ImVec4 color = ImGui::GetStyleColorVec4(selected ? ImGuiCol_HeaderHovered : ImGuiCol_HeaderActive);
					ImU32 select_color = ImGui::ColorConvertFloat4ToU32(color);

					draw_list->AddRectFilled(select_min, select_max, select_color, 0);
				}

				pos = ImGui::GetCursorPos();

				bool active = m_EntityID->IsActive();
				if (!active)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25f);
				}

				bool temp = m_EntityID->IsActive();
				ImGui::SetNextItemAllowOverlap();
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				if (ImGui::Checkbox(IMGUI_FORMAT_ID("", CHECKBOX_ID, string_extensions::StringToUpper(m_EntityID->GetName()) + "_HIERARCHY").c_str(), &temp))
				{
					m_EntityID->SetActive(temp);
				}
				ImGui::PopStyleVar();

				ImGui::SetCursorPos(ImVec2(pos.x + 30, pos.y));
				ImGui::Text(GetIcon().c_str());
				ImGui::SetCursorPos(ImVec2(pos.x + 65, pos.y));
				ImGui::Text(m_EntityID->GetName().c_str());

				clicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left) && is_hovered;

				if (!active)
				{
					ImGui::PopStyleVar();
				}
			}

			void EntityUIView::RenderSelectable()
			{
				ImVec2 toolbarSize = ImVec2(ImGui::GetContentRegionAvail().x, core::ENGINE.GetEditor().GetImGuiWindow().HeaderSize().y * 2.5f);
				ImGui::BeginToolbar(toolbarSize);

				ImVec2 padding = core::ENGINE.GetEditor().GetImGuiWindow().GetWindowPadding();

				float fontSize = core::ENGINE.GetEditor().GetImGuiWindow().FontSize();

				float y = ImGui::GetCursorPosY();
				float x = ImGui::GetCursorPosX() + fontSize;
				ImGui::SetCursorPos(ImVec2(x, y + fontSize));
				ImGui::PushFont(core::ENGINE.GetEditor().GetImGuiWindow().CapitalIcon());
				ImGui::Text(GetIcon().c_str());
				ImGui::PopFont();

				ImGui::SetCursorPosY(y + (fontSize / 2));
				ImGui::SetCursorPosX(x + (fontSize * 3));

				m_NameInput.SetString(m_EntityID->GetName());
				if (m_NameInput.Render(imgui::IMGUI_FORMAT_ID("", INPUT_ID, "NAME_INSPECTOR").c_str(), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					m_EntityID->SetName(m_NameInput.GetString());
				}

				ImGui::SetCursorPosY(y + (core::ENGINE.GetEditor().GetImGuiWindow().HeaderSize().y * 1.5f));
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().ItemSpacing.x);

				bool temp = m_EntityID->IsActive();
				if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(temp ? ICON_FA_CHECKMARK_CHECKED : ICON_FA_CHECKMARK, CHECKBOX_ID, string_extensions::StringToUpper(m_EntityID->GetName()) + "_INSPECTOR").c_str(), &temp, ImVec2(0, core::ENGINE.GetEditor().GetImGuiWindow().HeaderSize().y)))
				{
					m_EntityID->SetActive(temp);
				}
				ImGui::SameLine();
				if (ImGui::TransparentButton(IMGUI_FORMAT_ID(std::string(ICON_FA_DELETE), BUTTON_ID, "DELETE_INSPECTOR").c_str(), ImVec2(0, core::ENGINE.GetEditor().GetImGuiWindow().HeaderSize().y)))
				{
					// TODO: This is still filthy af. Think of a better way.
					core::ENGINE.GetECS().DeleteEntity(*m_EntityID);
					core::ENGINE.GetEditor().GetImGuiWindow().GetHierarchyWindow().m_NeedsRefresh = true;
					m_EntityID = nullptr;
				}
				ImGui::SameLine();
				gameplay::EntityComponentSystem& ecs = core::ENGINE.GetECS();
				gameplay::TransformSystem& transformSys = ecs.GetSystem<gameplay::TransformSystem>();
				if (ImGui::TransparentButton(IMGUI_FORMAT_ID(std::string(ICON_FA_DELETE), BUTTON_ID, "TEST_INSPECTOR").c_str(), ImVec2(0, core::ENGINE.GetEditor().GetImGuiWindow().HeaderSize().y)))
				{
					transformSys.CreateComponent(*m_EntityID);
					core::ENGINE.GetEditor().SetSelectable(this);
				}

				ImGui::EndToolbar(padding);

				if (!m_EntityID)
				{
					return;
				}

				for (ComponentUIView* component : m_Components)
				{
					component->Render();
				}
			}

			void EntityUIView::Select()
			{
				gameplay::EntityComponentSystem& ecs = core::ENGINE.GetECS();
				gameplay::TransformSystem& transformSys = ecs.GetSystem<gameplay::TransformSystem>();
				if (ecs.GetSystem<gameplay::TransformSystem>().IsIdHere(*m_EntityID))
				{
					m_Components.push_back(new TransformComponentUIView(m_Window, *m_EntityID->transform()));
				}
			}

			void EntityUIView::Deselect()
			{
				for (ComponentUIView* component : m_Components)
				{
					delete component;
				}
				m_Components.clear();
			}
		}
	}
}

#endif // __EDITOR__