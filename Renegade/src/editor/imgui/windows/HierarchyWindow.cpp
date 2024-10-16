#ifdef __EDITOR__

#include "editor/imgui/windows/HierarchyWindow.h"

#include "editor/imgui/ImGuiWindow.h"
#include "editor/imgui/ImGuiDefines.h"
#include "imgui/imgui_helpers.h"
#include "logger/Logger.h"
#include "utils/string_extensions.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			HierarchyWindow::HierarchyWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_HIERARCHY) + " Hierarchy", "Hierarchy")
			{ }

			void HierarchyWindow::Render()
			{
				//if (m_NeedsRefresh)
				//{
				//	//m_FilteredItems.clear();

				//	m_NeedsRefresh = false;

				//	//searchString = string_extensions::StringToLower(std::string(SEARCHSTRING));
				//	//bool isEmptyString = searchString.empty();

				//	//for (auto& item : DEBUG_ITEMS)
				//	//{
				//	//	if (isEmptyString || string_extensions::StringToLower(item.Name()).find(searchString) != std::string::npos)
				//	//	{
				//	//		m_FilteredItems.push_back(&item);
				//	//	}
				//	//}
				//}

				ImVec2 toolbarSize = ImVec2(ImGui::GetContentRegionAvail().x, m_Window.HeaderSize().y);
				ImGui::BeginToolbar(toolbarSize);

				ImGui::EndToolbar(m_Window.GetWindowPadding());

				//ImDrawList* draw_list = ImGui::GetWindowDrawList();

				//// Define the position and size of your button
				//ImVec2 button_pos = ImGui::GetCursorScreenPos();
				//ImVec2 button_size = ImVec2(ImGui::GetContentRegionAvail().x, 32); // Width, Height

				//// Calculate the rectangle for the button
				//ImVec2 button_min = button_pos;
				//ImVec2 button_max = ImVec2(button_pos.x + button_size.x, button_pos.y + button_size.y);

				//draw_list->AddRectFilled(button_min, button_max, ImGui::ImColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Button)), 0);  // 10.0f is the rounding radius

				//if (ImGui::TransparentButton(IMGUI_FORMAT_ID(ICON_FA_ADD, BUTTON_ID, "ADD_MENU_HIERARCHY").c_str()))
				//{ }

				//ImGui::SameLine();
				//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 100);
				//float windowWidth = ImGui::GetContentRegionAvail().x - 25;

				//if (ImGui::SearchBar(m_Window.FontSize(), IMGUI_FORMAT_ID("", INPUT_ID, "SEARCH_ENTITIES_HIERARCHY").c_str(), SEARCHSTRING, sizeof(SEARCHSTRING), ImVec2(windowWidth, 0)))
				//{
				//	m_NeedsRefresh = true;
				//}

				//if (ImGui::BeginChild(IMGUI_FORMAT_ID("", CHILD_ID, "ENTITY_LIST_HIERARCHY").c_str()))
				//{
				//	m_ShowContextMenu = false;

				//	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

				//	if (!searchString.empty())
				//	{
				//		//for (HierarchyItemView* itemPtr : m_FilteredItems)
				//		//{
				//		//	if (!itemPtr)
				//		//	{
				//		//		continue;
				//		//	}

				//		//	HierarchyItemView& item = *itemPtr;
				//		//	item.Render(*this);
				//		//}
				//	}
				//	else
				//	{
				//		int move_from = -1, move_to = -1;
				//		for (int n = 0; n < DEBUG_ITEMS.size(); n++)
				//		{
				//			DEBUG_ITEMS[n].Render(*this);

				//			if (m_IsRenaming)
				//			{
				//				continue;
				//			}
				//			ImGuiDragDropFlags src_flags = 0;
				//			src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
				//			src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;
				//			src_flags |= ImGuiDragDropFlags_AcceptNoPreviewTooltip;

				//			if (ImGui::BeginDragDropSource(src_flags))
				//			{
				//				ImGui::SetDragDropPayload("ENTITY_LIST_DND_HIERARCHY", &n, sizeof(int));
				//				ImGui::EndDragDropSource();
				//				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
				//			}

				//			if (ImGui::BeginDragDropTarget())
				//			{
				//				ImGuiDragDropFlags target_flags = 0;
				//				target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
				//				target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
				//				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_LIST_DND_HIERARCHY", target_flags))
				//				{
				//					move_from = *(const int*)payload->Data;
				//					move_to = n;
				//				}
				//				ImGui::EndDragDropTarget();
				//			}
				//		}
				//		if (!m_IsRenaming && move_from != -1 && move_to != -1)
				//		{
				//			int copy_dst = (move_from < move_to) ? move_from : move_to + 1;
				//			int copy_src = (move_from < move_to) ? move_from + 1 : move_to;
				//			int copy_count = (move_from < move_to) ? move_to - move_from : move_from - move_to;
				//			auto tmp = DEBUG_ITEMS[move_from];
				//			auto tmp2 = DEBUG_ITEMS[move_to];
				//			DEBUG_ITEMS[move_to] = tmp;
				//			DEBUG_ITEMS[move_from] = tmp2;
				//			ImGui::SetDragDropPayload("ENTITY_LIST_DND_HIERARCHY", &move_to, sizeof(int));
				//		}
				//	}
				//	ImGui::PopStyleVar();

				//	if (m_ShowContextMenu)
				//	{
				//		ImGui::OpenPopup(IMGUI_FORMAT_ID("", POPUP_WINDOW_ID, "CONTEXT_MENU_HIERARCHY").c_str());
				//	}

				//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
				//	if (SELECTED_HIERARCHY_ITEM != -1 && ImGui::BeginPopup(IMGUI_FORMAT_ID("", POPUP_WINDOW_ID, "CONTEXT_MENU_HIERARCHY").c_str()))
				//	{
				//		ImVec4 textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
				//		textColor.w = 0.5f;
				//		ImGui::TextColored(textColor, DEBUG_ITEMS[SELECTED_HIERARCHY_ITEM].Name().c_str());
				//		if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_COPY) + " Copy", MENU_ITEM_ID, "COPY_ITEM_HIERARCHY").c_str()))
				//		{
				//			// TODO: Copy.
				//		}
				//		if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_PASTE) + " Paste", MENU_ITEM_ID, "PASTE_ITEM_HIERARCHY").c_str()))
				//		{
				//			// TODO: Paste.
				//		}
				//		ImGui::Separator();
				//		if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_RENAME) + " Rename", MENU_ITEM_ID, "RENAME_ITEM_HIERARCHY").c_str()))
				//		{
				//			m_IsRenaming = true;
				//		}
				//		if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_COPY) + " Duplicate", MENU_ITEM_ID, "DUPLICATE_ITEM_HIERARCHY").c_str()))
				//		{
				//			// TODO: Duplicate.
				//		}
				//		if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_DELETE) + " Delete", MENU_ITEM_ID, "DELETE_ITEM_HIERARCHY").c_str()))
				//		{
				//			DEBUG_ITEMS.erase(DEBUG_ITEMS.begin() + SELECTED_HIERARCHY_ITEM);
				//		}

				//		ImGui::EndPopup();
				//	}
				//	ImGui::PopStyleVar();

				//	ImGui::EndChild();
				//}
			}

			void HierarchyWindow::SetShowContextMenu(bool a_ShowContextMenu)
			{
				m_ShowContextMenu = a_ShowContextMenu;
			}

			void HierarchyWindow::SetIsRenaming(bool a_IsRenaming)
			{
				m_IsRenaming = a_IsRenaming;
			}

			bool HierarchyWindow::ShowContextMenu() const
			{
				return m_ShowContextMenu;
			}

			bool HierarchyWindow::IsRenaming() const
			{
				return m_IsRenaming;
			}
		}
	}
}

#endif // __EDITOR__