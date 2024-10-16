#ifdef __EDITOR__

#include "editor/imgui/windows/ExplorerWindow.h"

#include "editor/imgui/ImGuiDefines.h"
#include "utils/string_extensions.h"
#include "file/FileLoader.h"
#include "editor/imgui/ImGuiWindow.h"
#include "imgui/imgui_helpers.h"
#include "editor/Editor.h"
#include "core/Engine.h"
#include "editor/ExplorerResource.h"
#include "editor/imgui/ExplorerResourceUIView.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			ExplorerWindow::ExplorerWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_EXPLORER) + " Explorer", "Explorer")
			{ }

			bool ExplorerWindow::Initialize()
			{
				m_AssetRoot.SetResource(&core::ENGINE.GetEditor().GetAssetDatabase().m_Root);
				SetExplorerRoot(&m_AssetRoot);
				return true;
			}

			void ExplorerWindow::SetExplorerRoot(ExplorerResourceUIView* a_Resource)
			{
				if (!a_Resource)
				{
					return;
				}

				m_FolderRoot = a_Resource;
				m_FolderRoot->GetChildren();
			}

			void ExplorerWindow::RenderFolder(ExplorerResourceUIView& a_Resource)
			{
				if (a_Resource.m_Name.empty() ||
					a_Resource.m_Resource->m_Path.empty() ||
					!a_Resource.m_Show)
				{
					return;
				}

				if (a_Resource.m_Resource->m_ResourceType != ExplorerResourceType::Folder)
				{
					return;
				}

				std::string name = a_Resource.m_Name;
				std::string icon = ICON_FA_FOLDER_OPEN;
				if (a_Resource.m_Resource->m_ResourceType == ExplorerResourceType::Folder && a_Resource.m_FoldedOut && a_Resource.HasFolders())
				{
					icon = ICON_FA_FOLDER_OPEN;
				}
				else if (a_Resource.m_Resource->m_ResourceType == ExplorerResourceType::Folder)
				{
					icon = ICON_FA_FOLDER;
				}
				std::string id = IMGUI_FORMAT_ID("", TREE_NODE_ID, "TREE_NODE_" + string_extensions::StringToUpper(a_Resource.m_Resource->m_Path) + "_EXPLORER");

				bool clicked, right_clicked;

				int flags = ImGuiTreeNodeFlags_OpenOnArrow;
				if (!a_Resource.HasFolders())
				{
					flags = ImGuiTreeNodeFlags_Leaf;
				}
				const bool fold = ImGui::EngineTreeNodeExS(id.c_str(), icon.c_str(), name.c_str(), clicked, right_clicked, &a_Resource == m_FolderRoot, ImVec2(ImGui::GetContentRegionAvail().x, m_Window.FontSize()), flags);

				if (right_clicked)
				{
					m_ShowContextMenu |= true;
					m_SelectedResource = &a_Resource;
				}

				if (clicked)
				{
					m_NewFolderRoot = &a_Resource;
					m_SelectedResource = &a_Resource;
				}

				if (a_Resource.m_FoldedOut != fold)
				{
					a_Resource.m_FoldedOut = fold;
					if (a_Resource.m_FoldedOut)
					{
						a_Resource.GetChildren();
					}
				}

				if (a_Resource.m_FoldedOut)
				{
					for (auto& resource : a_Resource.m_Resources)
					{
						RenderFolder(resource);
					}
				}

				if (fold)
				{
					ImGui::TreePop();
				}
			}

			void ExplorerWindow::Render()
			{
				// If the asset database is not ready, do not continue.
				if (!core::ENGINE.GetEditor().GetAssetDatabase().Ready())
				{
					return;
				}

				// This needs to be done at the start of the frame to avoid errors.
				// We set the root directory of the second window that shows the assets.
				if (m_NewFolderRoot)
				{
					if (ExplorerResourceUIView* derivedPtr = dynamic_cast<ExplorerResourceUIView*>(core::ENGINE.GetEditor().GetAssetDatabase().m_EditorSelectable))
					{
						core::ENGINE.GetEditor().GetAssetDatabase().m_EditorSelectable = nullptr;
					}

					m_SelectedResource = nullptr;
					SetExplorerRoot(m_NewFolderRoot);
					m_NeedsRefresh = true;
					m_NewFolderRoot = nullptr;
				}

				// This needs to be done at the start of the frame to avoid errors.
				// We rescan the database here.
				if (m_NeedsRescan)
				{
					if (ExplorerResourceUIView* derivedPtr = dynamic_cast<ExplorerResourceUIView*>(core::ENGINE.GetEditor().GetAssetDatabase().m_EditorSelectable))
					{
						core::ENGINE.GetEditor().GetAssetDatabase().m_EditorSelectable = nullptr;
					}

					m_AssetRoot.SetResource(&core::ENGINE.GetEditor().GetAssetDatabase().m_Root);
					SetExplorerRoot(&m_AssetRoot);
					m_NeedsRefresh = true;
					m_NeedsRescan = false;
				}

				// This needs to be done at the start of the frame to avoid errors.
				// We refresh the assets that show up based on the searchbar and the root directory.
				if (m_NeedsRefresh)
				{
					m_ResourcesToShow.clear();

					m_NeedsRefresh = false;

					bool isEmptyString = m_SearchBar.GetString().empty();

					for (auto& resource : m_FolderRoot->m_Resources)
					{
						if (isEmptyString || string_extensions::StringToLower(resource.m_Name).find(m_SearchBar.GetString()) != std::string::npos)
						{
							m_ResourcesToShow.push_back(&resource);
						}
					}
				}
				if (!m_FolderRoot)
				{
					return;
				}


				m_ShowContextMenu = false;

				ImVec2 toolbarSize = ImVec2(ImGui::GetContentRegionAvail().x, m_Window.HeaderSize().y);
				ImGui::BeginToolbar(toolbarSize);

				if (ImGui::TransparentButton(
					IMGUI_FORMAT_ID(std::string(ICON_FA_REFRESH), BUTTON_ID, "RESCAN_EXPLORER").c_str(), ImVec2(0, toolbarSize.y)))
				{
					m_NeedsRescan = true;
				}

				ImGui::SameLine();

				float searchbarWidth = 200;
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (searchbarWidth + m_Window.GetWindowPadding().x));
				if (m_SearchBar.Render(IMGUI_FORMAT_ID("", INPUT_ID, "SEARCHBAR_EXPLORER").c_str(), ImVec2(searchbarWidth, toolbarSize.y), m_Window.GetFramePadding().x / 2))
				{
					m_NeedsRefresh = true;
				}

				ImGui::EndToolbar(m_Window.GetWindowPadding());

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(m_Window.GetFramePadding().x, m_Window.GetFramePadding().y));
				if (ImGui::BeginChild(
					IMGUI_FORMAT_ID("", CHILD_ID, "DIRECTORIES_EXPLORER").c_str(),
					ImVec2(
						(ImGui::GetContentRegionAvail().x - m_Window.GetWindowPadding().x) * 0.15f,
						ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y
					),
					ImGuiChildFlags_Borders
				))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					RenderFolder(m_AssetRoot);
					ImGui::PopStyleVar();
				}
				ImGui::EndChild();

				ImGui::PopStyleVar();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(m_Window.GetFramePadding().x * 2, m_Window.GetFramePadding().y * 2));
				ImGui::SameLine();
				if (ImGui::BeginChild(
					IMGUI_FORMAT_ID("", CHILD_ID, "FILES_EXPLORER").c_str(),
					ImVec2(
						ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x,
						ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y
					),
					ImGuiChildFlags_Borders
				))
				{
					if (m_FolderRoot)
					{
						if (ImGui::BeginChild(
							IMGUI_FORMAT_ID("", CHILD_ID, "FILES_INNER_EXPLORER").c_str(),
							ImVec2(
								0,
								0
							)
						))
						{
							if (m_FolderRoot->m_Parent)
							{
								bool clicked, right_clicked, double_clicked;
								ImGui::EngineResourceNode(
									IMGUI_FORMAT_ID("", SELECTABLE_ID, "PREVIOUS_FOLDER_EXPLORER").c_str(),
									ICON_FA_FOLDER,
									"../",
									"",
									clicked,
									right_clicked,
									double_clicked,
									false
								);

								// TODO: Get parent.
								if (double_clicked)
								{
									m_NewFolderRoot = m_FolderRoot->m_Parent;
								}
							}

							for (ExplorerResourceUIView* item : m_ResourcesToShow)
							{
								if (!item)
								{
									continue;
								}

								bool clicked, right_clicked, double_clicked;
								item->Render(clicked, right_clicked, double_clicked, item == core::ENGINE.GetEditor().GetAssetDatabase().m_EditorSelectable);

								if (clicked && item->m_Resource->m_ResourceType != ExplorerResourceType::Folder)
								{
									core::ENGINE.GetEditor().GetAssetDatabase().m_EditorSelectable = item;
								}
								if (right_clicked)
								{
									m_SelectedResource = item;
									m_ShowContextMenu |= true;
								}
								if (double_clicked && item->m_Resource->m_ResourceType == ExplorerResourceType::Folder)
								{
									m_NewFolderRoot = item;
								}
							}
						}
						ImGui::EndChild();
					}
				}
				ImGui::EndChild();
				ImGui::PopStyleVar();

				if (m_ShowContextMenu)
				{
					ImGui::OpenPopup(IMGUI_FORMAT_ID("", POPUP_WINDOW_ID, "RESOURCE_OPTIONS_EXPLORER").c_str());
				}

				//auto createNewFolder = [](const ExplorerResource& a_Folder)
				//	{
				//		std::string name = ExplorerResource::GetUniqueName(a_Folder, "New Folder");
				//		bool success = file::FileLoader::CreateFolder(a_Folder.m_Path + "/" + name);
				//		return success;
				//	};

				//if (m_SelectedResource)
				//{
				//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(m_Window.GetFramePadding().x * 2, m_Window.GetFramePadding().y * 2));
				//	if (ImGui::BeginPopup(IMGUI_FORMAT_ID("", POPUP_WINDOW_ID, "RESOURCE_OPTIONS_EXPLORER").c_str()))
				//	{
				//		ImVec4 textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
				//		textColor.w = 0.5f;
				//		ImGui::TextColored(textColor, m_SelectedResource->m_Name.c_str());

				//		if (m_SelectedResource->m_ResourceType != ExplorerResourceType::Folder)
				//		{
				//			ImGui::Separator();
				//			if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_ARROW_TURN_UP) + " Show in explorer", MENU_ITEM_ID, "SHOW_FILE_IN_EXPLORER_EXPLORER").c_str()))
				//			{
				//				file::FileLoader::OpenInExplorer(m_SelectedResource->m_Parent->m_Path.c_str());
				//			}
				//			ImGui::Separator();
				//			if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_DELETE) + " Delete note", MENU_ITEM_ID, "DELETE_FILE_EXPLORER").c_str()))
				//			{
				//				m_SelectedResource->Delete();
				//				m_NeedsRescan = true;
				//			}
				//		}
				//		else
				//		{
				//			ImGui::Separator();

				//			if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_FOLDER_PLUS) + " New folder", MENU_ITEM_ID, "CREATE_NEW_FOLDER").c_str()))
				//			{
				//				if (createNewFolder(*m_SelectedResource))
				//				{
				//					m_NeedsRescan = true;
				//				}
				//			}
				//			ImGui::Separator();
				//			if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_ARROW_TURN_UP) + " Show in explorer", MENU_ITEM_ID, "SHOW_FOLDER_IN_EXPLORER").c_str()))
				//			{
				//				file::FileLoader::OpenInExplorer(m_SelectedResource->m_Parent->m_Path.c_str());
				//			}
				//			ImGui::Separator();
				//			if (ImGui::MenuItem(IMGUI_FORMAT_ID(std::string(ICON_FA_DELETE) + " Delete", MENU_ITEM_ID, "DELETE_FILE_EXPLORER").c_str()))
				//			{
				//				m_SelectedResource->Delete();
				//				m_NeedsRescan = true;
				//			}
				//		}
				//		ImGui::EndPopup();
				//	}
				//	ImGui::PopStyleVar();
				//}
			}
		}
	}
}

#endif // __EDITOR__